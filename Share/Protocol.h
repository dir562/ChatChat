#pragma once


#include "types.h"
#include "enum.h"

//====================================

using packet_size_t = int8;

//====================================

const uint16_t SERVER_PORT = 8282;
const auto SERVER_IP = "127.0.0.1";

//====================================

const int MAX_NAME_SIZE = 10;
const int MAX_PLAYER = 20;
const int MAX_ENEMY = 30;
const int MAX_OBJECT = MAX_PLAYER + MAX_ENEMY;
const int MAX_PACKET_SIZE = 256;

//====================================

#pragma warning(push)
#pragma warning(disable: 26812)

BETTER_ENUM
(
	PAKCET_TYPE, int8

	, NONE = 0
	
	/* Client 2 Server */
	
	, CS_NONE			= 10
	, CS_TRY_LOGIN
	, CS_LOGIN_PACKET
	, CS_MAKE_MAP_DONE
	, CS_NEW_CHARATOR




	/* Server 2 Client */

	, SC_NONE			= 100





);

#pragma warning(pop)


#pragma pack (push, 1)
//================ BASE ================


template<class T>
struct packet_base
{
	packet_size_t size = sizeof(T);
	PAKCET_TYPE packet_type = +PAKCET_TYPE::_from_string_nocase(typeid(T).name() + 7);
};
#define PACKET(name) struct name : packet_base<name>											

PACKET(none)
{
};

//=============== LOG_IN =================

// => 이 이름으로 로그인 할래
PACKET(cs_try_login)
{
	char name[MAX_NAME_SIZE];
};


// => ok 너의 id는 이거야

PACKET(sc_ok_login)
{
	int8 login_id;
};

// 맵에 누구누구가 어디어디에 있는지 알려주어야 함
// PACKET(sc_new_charator) * numofchractor; to new player

PACKET(sc_new_charator)
{
	int8 login_id;
	char name[MAX_NAME_SIZE];
	float x;
	float y;
};

PACKET(cs_make_map_done)
{

};

// 다른 클라이언트들 한테 새 캐릭터 로그인알림
// PACKET(sc_new_charator); to old players

//============= LOG_OUT ===========

// => logout (esc 누름)

PACKET(sc_logout)
{
	int8 logout_id;
};

// => recv 의 리턴값이 0 인 경우 또는 PACKET(sc_logout) 패킷이 왔을 경우.

PACKET(cs_logout)
{
	int8 logout_id;
};

// => 맵에서 플레이어 제거.

//=============== MOVE_INPUT =================

// 이동연잔자들, 비트연산으로 press,unpress 설정
enum class MOVE_DIR : int8
{
	FORWARD = 1 << 0,
	BACK = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3
};

// => input 이 변화했

PACKET(cs_moved)
{
	MOVE_DIR arrow_key;
};

// => 다른플레이어들한테 전송

PACKET(sc_moved)
{
	int8 mover_id;
	MOVE_DIR arrow_key;
};

//=============== CHATTING =================

// => 채팅 enter

PACKET(cs_chat)
{
	char chat[30];
	char padding = '\0';
};

// => 다른 플레이어들한테 전송

PACKET(sc_chat)
{
	int8 chatter_id;
	char chat[30];
	char padding = '\0';
};

//============= 타격 ===========

// => 플레이어의 타격시도

PACKET(sc_try_attack)
{
	int8 defender_id;
};

// => 서버에서 위치와 접속여부를 토대로 검증

PACKET(cs_attacked)
{
	int8 attacker_id;
	int8 defender_id;
};

// => attacket 의 공격 이펙트 발동
// => defender 의 피격 이펙트 발동



//==================== MODEL_CHANGE ==========================

// => 변경할래

PACKET(cs_model_change)
{
	int8 model_id;
};

// => ok, 모든 플레이어들한테 전송

PACKET(sc_model_change)
{
	int8 id;
	int8 model_id;
};

// => 적용.

//================================================


#pragma pack(pop)
