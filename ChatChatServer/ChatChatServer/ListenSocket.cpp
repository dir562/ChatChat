#include "stdafx.h"
#include "IocpHelper.h"
#include "ListenSocket.h"


void ListenSocket::Init(HANDLE iocp)
{
	listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr; ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = ::htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	auto res = ::bind(listen_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	res = ::listen(listen_socket_, SOMAXCONN);
	::CreateIoCompletionPort(reinterpret_cast<HANDLE>(listen_socket_), iocp, 0, 0);
};


ListenSocket::~ListenSocket()
{
	::closesocket(listen_socket_);
	REPORT_ERROR("Maybe ServerDown");
}

void ListenSocket::do_accept()
{
	newface_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&accept_ex_.wsa_over, sizeof(accept_ex_.wsa_over));
	accept_ex_.comp_op = COMP_OP::OP_ACCEPT;
	constexpr auto len = sizeof(SOCKADDR_IN) + 16;
	*reinterpret_cast<SOCKET*>(accept_ex_.net_buf) = newface_socket_;
	BOOL res = ::AcceptEx(listen_socket_, newface_socket_, accept_buf_, 0, len, len, nullptr, &accept_ex_.wsa_over);
	if (false == res)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())SocketUtil::DisplayError(WSAGetLastError());
	}
	cerr << "accept_start" << endl;
}
