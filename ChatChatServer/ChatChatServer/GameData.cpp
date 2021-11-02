#include "stdafx.h"
#include "IOCP.h"
#include "GameData.h"


// ============================================= //

void process_packet(const NetID net_id, const char* const packet)
{
	const packet_base<void>* pck_base = reinterpret_cast<const packet_base<void>*>(packet);
	const packet_size_t pck_size = pck_base->size;
	const PAKCET_TYPE pck_type = pck_base->packet_type;

	switch (pck_type)
	{
	case PAKCET_TYPE::CS_CHAT:
	{
		auto pck = reinterpret_cast<const cs_chat*>(packet);
		cout << (int)net_id << "::" << pck->chat << endl;

		sc_chat react_pck;
		react_pck.chatter_id = net_id;
		memcpy(react_pck.chat, pck->chat, sizeof(sc_chat::chat));

		auto& sessions = IOCP::get().get_sessions();
		for (auto& s : sessions)
		{
			if (s.check_state(SESSION_STATE::ST_ACCEPT))
			{
				s.do_send(&react_pck, react_pck.size);
			}
		}
	}
	CASE PAKCET_TYPE::CS_NONE: { }
	break; default: cerr << "couldn't be here!! PAKCET_TYPE ERROR ::" << pck_type << "::" << endl;
	}
}

