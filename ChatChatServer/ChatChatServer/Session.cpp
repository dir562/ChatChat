#include "stdafx.h"
#include "iocpHelper.h"
#include "IOCP.h"
#include "Session.h"



// ===========================================================

void Session::NewSession(SOCKET socket, NetID net_id) // ���ϼ� �����Լ� -> doAccept
{
	// already change state in IOCP::get_new_net_id
	// state_ = SESSION_STATE::ST_ACCEPT;
	net_id_ = net_id;
	socket_ = socket;
}

void Session::clear() // do_disconnect ������ ���. // ON_DICONNECT
{
	socket_ = INVALID_SOCKET;
	recv_over_.clear();
	recv_over_.comp_op = COMP_OP::OP_RECV;
	state_ = SESSION_STATE::ST_FREE;
	prerecv_size_ = 0;
	net_id_ = 0;
}

// ===========================================================

void Session::do_recv()
{
	int res; DWORD recv_flag = 0;
	{
		shared_lock read_lck{ connection_lock_ };		// read lock
		if (check_state_bad()) return;
		ZeroMemory(&recv_over_.wsa_over, sizeof(recv_over_.wsa_over));
		recv_over_.wsa_buf.buf = recv_over_.net_buf + prerecv_size_;
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

void Session::do_disconnect() // recv or send HandleIoError ���� �߻�. IOCP������ �߻�����.
{
	cerr << "lockggg_%%%disconnect::" << (int)net_id_ << endl;

	auto s = static_cast<DWORD>(socket_);

	unique_lock write_lck{ connection_lock_ };	// write lock
	state_ = SESSION_STATE::ST_ON_DICONNECT;
	
	EXP_OVER* ex_over = new EXP_OVER{ COMP_OP::OP_DISCONNECT};
	PostQueuedCompletionStatus(IOCP::get().get_iocp(), s, net_id_, &ex_over->wsa_over);
}

// ===========================================================

bool Session::HandleIoError(int res)
{
	if (SOCKET_ERROR == res) {
		auto err = WSAGetLastError();
		// WSAENOTCONN �� ���, �̹� disconnect �� ������ ���̴�.
		// send���� state_lock ���� ó���ϱ⿡ �߻� �� �� �ִ�.
		if (WSA_IO_PENDING != err)
		{
			cout << "err!!! ::" << err << "::" << endl;
			if (WSAENOBUFS == err)
			{
				cerr << "���� ���� �ִ�ġ, �޸� ����!" << endl;
				REPORT_ERROR("");
			}
			if (check_state_good())
			{
				cout << "disconnect::" << (int)this->net_id_ << "::" << endl;
				do_disconnect();
			}
			return true;
		}
	}
	return false;
}
