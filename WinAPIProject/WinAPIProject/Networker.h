#pragma once

#include "includes.h"

class Networker
{
	SINGLE_TON(Networker)
	{
		WSADATA WSAData;
		int res = WSAStartup(MAKEWORD(2, 3), &WSAData);
		if (SOCKET_ERROR == res)
		{
			SocketUtil::ReportError("InitWsa FAIL.");
			exit(-1);
		}

		socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		connect(SERVER_IP);
	}

private:
	~Networker()
	{
		::closesocket(socket_);
		WSACleanup();
	}

	void connect(const char* server_ip)
	{
		//server_ip = "192.168.120.70";
		SOCKADDR_IN server_addr; ZeroMemory(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = ::htons(SERVER_PORT);
		::inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

		char tcp_opt = 1;
		::setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&tcp_opt, sizeof(tcp_opt));

		auto ret = ::connect(socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

		if (SOCKET_ERROR == ret)
		{
			SocketUtil::ReportError("connect fail, maybe 서버 안 연 듯.");
			SocketUtil::DisplayError(WSAGetLastError());
			exit(-1);
		}
	}
public:
	void do_recv();

	void process_packet(const char* const packet);

	// use this func with async
	void do_send(const void* packet, size_t packet_len);

private:
	SOCKET socket_;
	array<char, MAX_PACKET_SIZE + 1> recv_buf_{};

};

