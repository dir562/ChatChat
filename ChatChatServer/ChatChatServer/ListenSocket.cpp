#include "stdafx.h"
#include "ListenSocket.h"


ListenSocket::ListenSocket()
{
	listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = ::htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	::bind(listen_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	::listen(listen_socket_, SOMAXCONN);
	::CreateIoCompletionPort(reinterpret_cast<HANDLE>(listen_socket_), iocp_, 0, 0);
};


ListenSocket::~ListenSocket()
{
	/// <summary>
	///  우아한 종료 도전중/////
	/// </summary>
}

void ListenSocket::do_accept()
{
	newface_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&accept_ex_._wsa_over, sizeof(accept_ex_._wsa_over));
	accept_ex_._comp_op = +COMP_OP::OP_ACCEPT;
	cout << "wait_for_accept" << endl;
	auto res = ::AcceptEx(listen_socket_, newface_socket_, accept_buf_, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL, &accept_ex_._wsa_over);
	*reinterpret_cast<SOCKET*>(accept_ex_._net_buf) = newface_socket_;
	cout << res << "non_blocking_accept_start" << endl;
}
