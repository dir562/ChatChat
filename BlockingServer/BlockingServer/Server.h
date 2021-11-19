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
				thread recv_thread([&]() { sessions_[i].do_recv(); });
				recv_thread.detach();
				goto CONTINUING_ACCEPT;
			}
		}

		::closesocket(new_socket);
		SocketUtil::ReportError("SESSIONS FULL");
		goto CONTINUING_ACCEPT;
	}

	void process_packet(const NetID net_id, const void* const packet)
	{
		auto packet_type = reinterpret_cast<const packet_base<void>*>(packet)->packet_type;
		switch (packet_type)
		{
		case PAKCET_TYPE::CS_HI:
		{
			cout << net_id << "::hi" << endl;
			sc_new_charactor new_charactor_packet;
			new_charactor_packet.netid = net_id;
			new_charactor_packet.hp = 7;
			new_charactor_packet.x = 0;
			new_charactor_packet.y = 0;
			for (auto& s : sessions_)
			{
				if (s.check_state(SESSION_STATE::disconnected))
				{
					continue;
				}

				if (s.get_net_id() == net_id)
				{
					sc_hi_ok hi_ok; hi_ok.your_netid = net_id;
					s.do_send(&hi_ok, sizeof(hi_ok));
				}
				else
				{
					s.do_send(&new_charactor_packet, sizeof(new_charactor_packet));
				}
			}
		}
		CASE PAKCET_TYPE::CS_MY_INFO :
		{
			auto p = reinterpret_cast<const cs_my_info*>(packet);
			sc_info info_packet;
			info_packet.netid = net_id;
			info_packet.x = p->x;
			info_packet.y = p->y;
			info_packet.hp = p->hp;

			for (auto& s : sessions_)
			{
				if (s.check_state(SESSION_STATE::disconnected))
				{
					continue;
				}

				if (s.get_net_id() == net_id)
				{
					continue;
				}

				s.do_send(&info_packet, sizeof(info_packet));
			}
		}
		break; default: break;
		}
	}

public:
	//GET_REF(packet_queue);
	GET_REF(sessions);

private:
	//concurrent_queue<pair<void*, NetID>> packet_queue_;
	array<Session, 20> sessions_;
};

