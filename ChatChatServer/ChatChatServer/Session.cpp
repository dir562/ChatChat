#include "stdafx.h"
#include "iocpHelper.h"
#include "Session.h"

void Session::do_recv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&recv_over_.wsa_over, sizeof(recv_over_.wsa_over));
	recv_over_.wsa_buf.buf = reinterpret_cast<char*>(recv_over_.net_buf) + prerecv_size_;
	recv_over_.wsa_buf.len = sizeof(recv_over_.net_buf) - prerecv_size_;
	int res = ::WSARecv(socket_, &recv_over_.wsa_buf, 1, 0, &recv_flag, &recv_over_.wsa_over, NULL);
	if (SOCKET_ERROR == res) {
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			REPORT_ERROR("");
			do_disconnect();
		}
	}
}

void Session::do_send(void* packet, size_t packet_len)
{
	EXP_OVER* ex_over = new EXP_OVER{ COMP_OP::OP_SEND, packet_len, packet };
	int res = ::WSASend(socket_, &ex_over->wsa_buf, 1, 0, 0, &ex_over->wsa_over, NULL);
	if (SOCKET_ERROR == res) {
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			REPORT_ERROR("");
			do_disconnect();
		}
	}
	cout << "send :: " << *(packet_size_t*)ex_over->net_buf << " bytes" << endl;
}

void Session::do_disconnect()
{
	if (INVALID_SOCKET != socket_)
	{
		state_ = SESSION_STATE::ST_FREE;
		socket_ = INVALID_SOCKET;
		auto res = ::closesocket(socket_);
		if (SOCKET_ERROR == WSAGetLastError())
		{
			// WSAEWOULDBLOCK;
			REPORT_ERROR("");
		}
	}
}
