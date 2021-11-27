#include "stdafx.h"
#include "Networker.h"
#include "CEventMgr.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CTestPlayer.h"
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
	PACKET_TYPE pck_type = pck_base->packet_type;

	switch (pck_type)
	{
	case PACKET_TYPE::NONE:
	{
	}
	CASE PACKET_TYPE::SC_HI_OK :
	{
		auto pck = reinterpret_cast<const sc_hi_ok*>(packet);
		//=pck->your_netid 내 넷아이디로 등록
		cout << "!!!!0" << endl;
		CEventMgr::GetInst()->CallCreatePlayer((int)pck->your_netid, 7, 200.f + (float)WIN_X / 2.f, 255.5f + (float)WIN_Y / 2.f);

	}
	CASE PACKET_TYPE::SC_INFO :
	{
		auto pck = reinterpret_cast<const sc_info*>(packet);
		// 캐릭터의정보, 이미 있는경우, 없다가 생긴경우, 내 정보 모두 포함
		auto PLAYER = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::PLAYER)[0];

		if (pck->netid == PLAYER->GetID()) {
			//
		}
		auto player = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::OTHERPLAYER);
		auto p = find_if(player.begin(), player.end(), [&](CObj* o) { return o->GetID() == (int)pck->netid; });
		if (p == player.end()) {
			// 없던경우
			CEventMgr::GetInst()->CallCreateOtherPlayer((int)pck->netid, (int)pck->hp, pck->x, pck->y);
		}
		else {
			// 이미있는경우
		}

	}
	CASE PACKET_TYPE::SC_NEW_CHARACTOR :
	{
		// 새캐릭터의 접속! => 새캐릭터 생성
		auto pck = reinterpret_cast<const sc_new_charactor*>(packet);
		cout << (int)pck->netid << "::" << pck->x << ", " << pck->y << "::" << pck->hp << endl;
		CEventMgr::GetInst()->CallCreateOtherPlayer((int)pck->netid, (int)pck->hp, pck->x, pck->y	);
		cs_my_info my_info;
		auto PLAYER = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::PLAYER)[0];
		my_info.hp = dynamic_cast<CPlayer*>(PLAYER)->GetLife();
		my_info.x = PLAYER->GetPos().x;
		my_info.y = PLAYER->GetPos().y;
		// 내 정보 입력
		do_send(&my_info, sizeof(my_info));
	}
	CASE PACKET_TYPE::SC_HEART_BEAT:
	{
		cout << ".";
		static cs_heart_beat csheartbeat;
		do_send(&csheartbeat, sizeof(csheartbeat));
	}
	CASE PACKET_TYPE::SC_DISCONNECT :
	{
		auto pck = reinterpret_cast<const sc_disconnect*>(packet);
		cout << (int)pck->DisconnectID << "::" << "Disconnect" << endl;
		auto player = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::OTHERPLAYER);
		for (auto p : player) {
			if (p->GetID() == (int)pck->DisconnectID) {
				DeleteObject(p);
				return;
			}
		}
		cout << "ERROR!!!!" << (int)pck->DisconnectID << "is not exists" << endl;
	}
	CASE PACKET_TYPE::SC_KEY_INPUT:
	{
		auto pck = reinterpret_cast<const sc_key_input*>(packet);
		
		auto player = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::OTHERPLAYER);
		for (auto p : player) {
			if (p->GetID() == (int)pck->netid) {
				dynamic_cast<CTestPlayer*>(p)->MovingData(pck->key);
				return;
			}
		}
	}
	CASE PACKET_TYPE::SC_ATTACK:
	{
		auto pck = reinterpret_cast<const sc_attack*>(packet);
		auto Player= CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::PLAYER)[0];
		auto Otherplayer = CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::OTHERPLAYER);
		if ((int)pck->be_attacked_id == Player->GetID()) {
			for (auto p : Otherplayer) {
				if (p->GetID() == (int)pck->attacker_id) {
					dynamic_cast<CTestPlayer*>(p)->SetJumpPower(700.f);
					dynamic_cast<CPlayer*>(Player)->SetLifeandColor();
					return;
				}
			}
		}
		for (auto p : Otherplayer) {
			if (p->GetID() == (int)pck->attacker_id) {
				for (auto p2 : Otherplayer) {
					if ((int)pck->be_attacked_id == p2->GetID()) {
						if (dynamic_cast<CTestPlayer*>(p2)->GetLife() == 0) {
							return;
						}
						dynamic_cast<CTestPlayer*>(p)->SetJumpPower(700.f);
						dynamic_cast<CTestPlayer*>(p2)->SetLifeandColor();
						if (dynamic_cast<CTestPlayer*>(p2)->GetJumping()) {
							dynamic_cast<CTestPlayer*>(p2)->SetJumpPower(-100.f);
						}
					}
				}
			}

		}
		
		
	}


	break; default: break;
	}
}
