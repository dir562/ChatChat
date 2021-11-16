#include "../../Share/includes.h"


void InitWsa()
{
	WSADATA WSAData;
	int res = WSAStartup(MAKEWORD(2, 3), &WSAData);
	if (SOCKET_ERROR == res)
	{
		SocketUtil::ReportError("InitWsa FAIL.");
		exit(-1);
	}
}

void EndWsa()
{
	WSACleanup();
}

SOCKET g_socket;

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
	CASE PAKCET_TYPE::CS_NONE : { }
	break; default: 
		SocketUtil::DisplayError(WSAGetLastError());
		cerr << "###########couldn't be here!! PAKCET_TYPE ERROR ::" << pck_type << "::" << endl; 
				 // Beep(500, 2000);
	}
}

void do_recv()
{
	char buf[MAX_PACKET_SIZE + 1]{};
	auto prerecved = 0;
	while (true)
	{
		auto recved_bytes = recv(g_socket, buf + prerecved, MAX_PACKET_SIZE - prerecved, NULL);
		if (SOCKET_ERROR == recved_bytes)
		{
			SocketUtil::DisplayError(WSAGetLastError());
		}
		if (0 == recved_bytes)
		{
			cout << "disconnected server" << endl;
			closesocket(g_socket);
			exit(-1);
		}

		auto pck_start = buf;
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
			memmove(buf, pck_start, remain_bytes);
		}
	}
}

void do_send()
{
	while (true)
	{
		cs_test_chat packet;
		cin.getline(packet.chat, sizeof(packet.chat));
		auto res = send(g_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), NULL);
		if (SOCKET_ERROR == res)
		{
			SocketUtil::DisplayError(WSAGetLastError());
		}
	}
}

int main()
{
	cout << "press any key to start." << endl; int a; cin >> a;

	InitWsa();

	g_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN server_addr; ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = ::htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	char tcp_opt = 1;
	setsockopt(g_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&tcp_opt, sizeof(tcp_opt));

	connect(g_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

	thread listen_thread{ do_recv };

	do_send();

	listen_thread.join();
	EndWsa();
}