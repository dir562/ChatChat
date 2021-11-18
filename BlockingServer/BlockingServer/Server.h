#pragma once

#include "Session.h"
#include "ListenSocket.h"

class Server
{
	SINGLE_TON(Server)
	{
		WSADATA wsadata;
		auto res = WSAStartup(MAKEWORD(2, 2), &wsadata);
		CHECK_ERR_DISPLAY(res, "WSAStartup");
	}

	~Server()
	{
		WSACleanup();
	}

public:
	void do_accept()
	{
		auto& listen_socket_ = ListenSocket::get();

	CONTINUING_ACCEPT:
		auto new_socket = listen_socket_.accept();
		cout << "accept" << endl;
		for (int i = 0; i < sessions_.size(); i++)
		{
			if (sessions_[i].check_state(SESSION_STATE::disconnected))
			{
				cout << "new_session" << i << endl;
				sessions_[i].new_session(new_socket, i);
				thread recv_thread([&]() { sessions_[i].do_recv(packet_queue_); });
				recv_thread.detach();
				goto CONTINUING_ACCEPT;
			}
		}

		::closesocket(new_socket);
		SocketUtil::ReportError("SESSIONS FULL");
		goto CONTINUING_ACCEPT;
	}

	GET_REF(packet_queue);
	GET_REF(sessions);

private:
	concurrent_queue<pair<void*, NetID>> packet_queue_;
	array<Session, 20> sessions_;
};

