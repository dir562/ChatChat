#include "stdafx.h"
#include "iocpHelper.h"
#include "Session.h"



// ===========================================================

void Session::NewSession(SOCKET socket, NetID net_id) // ���ϼ� �����Լ� -> doAccept
{
	// already change state in IOCP::get_new_net_id
	// state_ = SESSION_STATE::ST_ACCEPT;
	net_id_ = net_id;
	socket_ = socket;
}

void Session::clear()
{
	ZeroMemory(this, sizeof(*this));
	socket_ = INVALID_SOCKET;
	recv_over_.clear();
	recv_over_.comp_op = COMP_OP::OP_RECV;
}

// ===========================================================

void Session::do_recv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&recv_over_.wsa_over, sizeof(recv_over_.wsa_over));
	recv_over_.wsa_buf.buf = reinterpret_cast<char*>(recv_over_.net_buf) + prerecv_size_;
	recv_over_.wsa_buf.len = sizeof(recv_over_.net_buf) - prerecv_size_;
	int res = ::WSARecv(socket_, &recv_over_.wsa_buf, 1, 0, &recv_flag, &recv_over_.wsa_over, NULL);
	HandleIoError(res);
}

void Session::do_send(void* packet, size_t packet_len)
{
	EXP_OVER* ex_over = new EXP_OVER{ COMP_OP::OP_SEND, packet_len, packet };
	int res = ::WSASend(socket_, &ex_over->wsa_buf, 1, 0, 0, &ex_over->wsa_over, NULL);
	HandleIoError(res);
}

void Session::do_disconnect()
{
	scoped_lock disconnect_lock{ connection_lock_ };

	if (INVALID_SOCKET != socket_)
	{
		state_ = SESSION_STATE::ST_FREE;
		auto res = ::closesocket(socket_);
		cerr << "disconnect::" << net_id_ << endl;
		if (SOCKET_ERROR == WSAGetLastError())
		{
			// WSAEWOULDBLOCK;
			REPORT_ERROR("");
		}
	}

	clear();
}

// ===========================================================

void Session::HandleIoError(int res)
{
	if (SOCKET_ERROR == res) {
		auto err = WSAGetLastError();
		// WSAENOTCONN �� ���, �̹� disconnect �� ������ ���̴�.
		// send���� state_lock ���� ó���ϱ⿡ �߻� �� �� �ִ�.
		if (WSA_IO_PENDING != err && WSAENOTCONN != err)
		{
			if (WSAENOBUFS == err)
			{
				cerr << "���� ���� �ִ�ġ, �޸� ����!" << endl;
			}
			REPORT_ERROR("");
			do_disconnect();
		}
	}
}
