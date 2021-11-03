#include "stdafx.h"
#include "iocpHelper.h"
#include "Session.h"



// ===========================================================

void Session::NewSession(SOCKET socket, NetID net_id) // 유일성 보장함수 -> doAccept
{
	// already change state in IOCP::get_new_net_id
	// state_ = SESSION_STATE::ST_ACCEPT;
	net_id_ = net_id;
	socket_ = socket;
}

void Session::clear() // do_disconnect 에서만 사용. // ON_DICONNECT
{
	exit(-1);
	ZeroMemory(this, sizeof(*this));
	socket_ = INVALID_SOCKET;
	recv_over_.clear();
	recv_over_.comp_op = COMP_OP::OP_RECV;
	state_ = SESSION_STATE::ST_FREE;
}

// ===========================================================

void Session::do_recv()
{
	int res; DWORD recv_flag = 0;
	{
		shared_lock read_lck{ connection_lock_ };		// read lock
		if (check_state_bad()) return;
		ZeroMemory(&recv_over_.wsa_over, sizeof(recv_over_.wsa_over));
		recv_over_.wsa_buf.buf = reinterpret_cast<char*>(recv_over_.net_buf) + prerecv_size_;
		recv_over_.wsa_buf.len = sizeof(recv_over_.net_buf) - prerecv_size_;
		res = ::WSARecv(socket_, &recv_over_.wsa_buf, 1, 0, &recv_flag, &recv_over_.wsa_over, NULL);
	}

	HandleIoError(res);
}

void Session::do_send(void* packet, size_t packet_len)
{
	int res;
	{
		shared_lock read_lck{ connection_lock_ };		// read lock
		if (check_state_bad()) return;
		EXP_OVER* ex_over = new EXP_OVER{ COMP_OP::OP_SEND, packet_len, packet };
		res = ::WSASend(socket_, &ex_over->wsa_buf, 1, 0, 0, &ex_over->wsa_over, NULL);
	}

	HandleIoError(res);
}

void Session::do_disconnect() // recv or send HandleIoError 에서 발생. IOCP에서도 발생가능.
{
	scoped_lock write_lck{ connection_lock_ };	// write lock

	if (INVALID_SOCKET != socket_)
	{
		// flush or no send
		auto res = ::closesocket(socket_);
		cerr << "disconnect::" << (int)net_id_ << endl;
		if (SOCKET_ERROR == WSAGetLastError())
		{
			// WSAEWOULDBLOCK;
			REPORT_ERROR("");
		}
	}

	clear();
}

// ===========================================================

bool Session::HandleIoError(int res)
{
	if (SOCKET_ERROR == res) {
		auto err = WSAGetLastError();
		// WSAENOTCONN 의 경우, 이미 disconnect 된 소켓일 것이다.
		// send에서 state_lock 없이 처리하기에 발생 할 수 있다.
		if (WSA_IO_PENDING != err && WSAENOTCONN != err)
		{
			if (WSAENOBUFS == err)
			{
				cerr << "서버 부하 최대치, 메모리 부족!" << endl;
			}
			REPORT_ERROR("");
			if (check_state_good())
			{
				do_disconnect();
			}
			return true;
		}
	}
	return false;
}
