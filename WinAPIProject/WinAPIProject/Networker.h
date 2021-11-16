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
	void do_recv()
	{
		auto prerecved = 0;
		while (true)
		{
			auto recved_bytes = recv(socket_, recv_buf_.data() + prerecved, MAX_PACKET_SIZE - prerecved, NULL);
			if (SOCKET_ERROR == recved_bytes)
			{
				SocketUtil::DisplayError(WSAGetLastError());
			}

			if (0 == recved_bytes)
			{
				cout << "disconnected server" << endl;
				exit(-1);
			}

			auto pck_start = recv_buf_.data();
			auto remain_bytes = recved_bytes + prerecved;

			// process completed packets.
			for (auto need_bytes = *reinterpret_cast<packet_size_t*>(pck_start);
				need_bytes <= remain_bytes;)
			{
				process_packet(pck_start);
				pck_start += need_bytes;
				remain_bytes -= need_bytes;
				need_bytes = *reinterpret_cast<packet_size_t*>(pck_start);

				if (0 == remain_bytes || 0 == need_bytes)
				{
					prerecved = 0;
					break;
				}
			}

			// remain_bytes가 남아있으면 미완성패킷의 데이터임. prerecv 해주기.
			if (0 != remain_bytes)
			{
				prerecved = remain_bytes;
				memmove(recv_buf_.data(), pck_start, remain_bytes);
			}
		}
	}

	void process_packet(const char* const packet)
	{
		const packet_base<void>* pck_base = reinterpret_cast<const packet_base<void>*>(packet);
		packet_size_t pck_size = pck_base->size;
		PAKCET_TYPE pck_type = pck_base->packet_type;

		switch (pck_type)
		{
		case PAKCET_TYPE::SC_TEST_HEART_BIT:
		{
			auto pck = reinterpret_cast<const sc_test_heart_bit*>(pck_base);
			cout << ".";
			//cout << boolalpha << "alive ::" << pck->time_after_send << "::" << endl;
		}
		CASE PAKCET_TYPE::SC_TEST_CHAT:
		{
			auto pck = reinterpret_cast<const sc_test_chat*>(pck_base);
			cout << (int)pck->chatter_id << "::" << pck->chat << endl;
		}
		break; default:
			SocketUtil::DisplayError(WSAGetLastError());
			cerr << "###########couldn't be here!! PAKCET_TYPE ERROR ::" << pck_type << "::" << endl;
			// Beep(500, 2000);
		}
	}

	// use this func with async
	void do_send(const void* packet)
	{
		const packet_base<void>* pck_base = reinterpret_cast<const packet_base<void>*>(packet);
		packet_size_t pck_size = pck_base->size;
		PAKCET_TYPE pck_type = pck_base->packet_type;

		//const vector<char> packet(pck_size);

		auto ret = ::send(socket_, reinterpret_cast<const char*>(packet), pck_size, NULL);
		
		if (SOCKET_ERROR == ret)
		{
			SocketUtil::ReportError("send error,,??");
			SocketUtil::DisplayError(WSAGetLastError());
			exit(-1);
		}
	}

private:
	SOCKET socket_;
	array<char, MAX_PACKET_SIZE + 1> recv_buf_{};

};

