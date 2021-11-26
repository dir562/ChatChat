#include "stdafx.h"
#include "ListenSocket.h"


ListenSocket::ListenSocket()
{
	listen_socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = ::htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	auto res = ::bind(listen_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	CHECK_ERR_DISPLAY(res, "bind");
	res = ::listen(listen_socket_, SOMAXCONN);
	CHECK_ERR_DISPLAY(res, "listen");
}

ListenSocket::~ListenSocket()
{
	::closesocket(listen_socket_);
	REPORT_ERROR("Maybe ServerDown");
}


SOCKET ListenSocket::accept()
{
	int addr_size{ sizeof(server_addr) };
	SOCKET newface_socket = ::accept(listen_socket_, reinterpret_cast<sockaddr*>(&server_addr), &addr_size);
	char x[30];
	inet_ntop(AF_INET, &server_addr.sin_addr, x, INET_ADDRSTRLEN);
	cout << x << endl;
	return newface_socket;
}

