#include "stdafx.h"
#include "Server.h"

void Server::process_packet(const NetID net_id, const void* const packet)
{
	auto SendtoOthers = [&](const auto& sendto_others_packet)
	{
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

			s.do_send(&sendto_others_packet, sizeof(sendto_others_packet));
		}
	};

	auto SendtoMeOthers = [&](const auto& sendto_me_packet, const auto& sendto_others_packet)
	{
		for (auto& s : sessions_)
		{
			if (s.check_state(SESSION_STATE::disconnected))
			{
				continue;
			}

			if (s.get_net_id() == net_id)
			{
				s.do_send(&sendto_me_packet, sizeof(sendto_me_packet));
			}
			else
			{
				s.do_send(&sendto_others_packet, sizeof(sendto_others_packet));
			}
		}
	};

	/////

	auto packet_type = reinterpret_cast<const packet_base<void>*>(packet)->packet_type;
	switch (packet_type)
	{
	case PACKET_TYPE::CS_HEART_BEAT:
	{
		cout << ".";
	}
	CASE PACKET_TYPE::CS_HI :
	{
		cout << (int)net_id << "::hi" << endl;
		sc_new_charactor new_charactor_packet;
		new_charactor_packet.netid = net_id;
		new_charactor_packet.hp = MAX_HP;
		new_charactor_packet.x = DEFAULT_X;
		new_charactor_packet.y = DEFAULT_Y;

		sc_hi_ok hi_ok; hi_ok.your_netid = net_id;

		SendtoMeOthers(hi_ok, new_charactor_packet);
	}
	CASE PACKET_TYPE::CS_MY_INFO :
	{
		auto p = reinterpret_cast<const cs_my_info*>(packet);
		sc_info info_packet;
		info_packet.netid = net_id;
		info_packet.x = p->x;
		info_packet.y = p->y;
		info_packet.hp = p->hp;

		SendtoOthers(info_packet);
	}
	CASE PACKET_TYPE::CS_KEY_INPUT :
	{
		auto p = reinterpret_cast<const cs_key_input*>(packet);
		sc_key_input key_input_packet;
		key_input_packet.netid = net_id;
		key_input_packet.key = p->key;

		SendtoOthers(key_input_packet);
	}
	CASE PACKET_TYPE::CS_ATTACK : /* 필요한가? */
	{
		auto p = reinterpret_cast<const cs_attack*>(packet);
		sc_attack attack_packet;
		attack_packet.attacker_id = net_id;
		attack_packet.be_attacked_id = p->be_attacked_id;

		SendtoOthers(attack_packet);
	}
	break; default: break;
	}
}
