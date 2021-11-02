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

void do_recv()
{
	char buf[MAX_PACKET_SIZE];
	while (true)
	{
		auto ret = recv(g_socket, buf, MAX_PACKET_SIZE, NULL);
		if (0 == ret || GetLastError())
		{
			cout << "disconnected server" << endl;
			closesocket(g_socket);
			exit(-1);
		}
		const packet_base<void>* pck_base = reinterpret_cast<const packet_base<void>*>(buf);
		const packet_size_t pck_size = pck_base->size;
		const PAKCET_TYPE pck_type = pck_base->packet_type;

		switch (pck_type)
		{
		case PAKCET_TYPE::SC_CHAT:
		{
			auto pck = reinterpret_cast<const sc_chat*>(buf);
			cout << (int)pck->chatter_id << "::" << pck->chat << endl;
		}
		CASE PAKCET_TYPE::CS_NONE : { }
		break; default: cerr << "couldn't be here!! PAKCET_TYPE ERROR ::" << pck_type << "::" << endl;
		}
	}
}

void do_send()
{
	while (true)
	{
		cs_chat packet;
		cin.getline(packet.chat, sizeof(packet.chat));
		send(g_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), NULL);
	}
}

int main()
{
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