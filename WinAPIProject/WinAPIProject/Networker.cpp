#include "stdafx.h"
#include "Networker.h"
#include "CEventMgr.h"

void Networker::do_recv()
{
	cs_hi hi_packet; do_send(&hi_packet, sizeof(hi_packet));

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


void Networker::do_send(const void* packet, size_t packet_len)
{
	auto ret = ::send(socket_, reinterpret_cast<const char*>(packet), packet_len, NULL);

	if (SOCKET_ERROR == ret)
	{
		SocketUtil::ReportError("send error,,??");
		SocketUtil::DisplayError(WSAGetLastError());
		exit(-1);
	}
}

void Networker::process_packet(const char* const packet)
{
	const packet_base<void>* pck_base = reinterpret_cast<const packet_base<void>*>(packet);
	packet_size_t pck_size = pck_base->size;
	PAKCET_TYPE pck_type = pck_base->packet_type;

	switch (pck_type)
	{
	case PAKCET_TYPE::NONE:
	{
	}
	CASE PAKCET_TYPE::SC_HI_OK :
	{
		auto pck = reinterpret_cast<const sc_hi_ok*>(packet);
		//=pck->your_netid 내 넷아이디로 등록
		CEventMgr::GetInst()->CallCreatePlayer((int)pck->your_netid, 7, 0, 0);

	}
	CASE PAKCET_TYPE::SC_INFO :
	{
		auto pck = reinterpret_cast<const sc_info*>(packet);
		// 캐릭터의정보, 이미 있는경우, 없다가 생긴경우, 내 정보 모두 포함
		CEventMgr::GetInst()->CallCreateOtherPlayer((int)pck->netid, 7, 0, 0);
	}
	CASE PAKCET_TYPE::SC_NEW_CHARACTOR :
	{
		// 새캐릭터의 접속! => 새캐릭터 생성
		auto pck = reinterpret_cast<const sc_new_charactor*>(packet);
		cout << (int)pck->netid << "::" << pck->x << ", " << pck->y << "::" << pck->hp << endl;

		cs_my_info my_info;
		// 내 정보 입력
		do_send(&my_info, sizeof(my_info));
	}

	break; default: break;
	}
}
