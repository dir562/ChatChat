#pragma once


#include "types.h"
#include "enum.h"
//====================================

using NetID = uint8;
using packet_size_t = uint8;

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

	, CS_NONE = 10
	, CS_TEST_CHAT
	, CS_TEST_MOVE
	, CS_MY_INFO
	, CS_HI
	, CS_HEART_BEAT

	/* Server 2 Client */

	, SC_NONE = 100
	, SC_TEST_CHAT
	, SC_HEART_BEAT
	, SC_NEW_CHARACTOR
	, SC_HI_OK
	, SC_INFO
	, SC_DISCONNECT

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

//=============== 위치정보 =================

PACKET(cs_hi)
{
};

PACKET(sc_hi_ok)
{
	NetID your_netid;
};

PACKET(cs_my_info)
{
	uint8 hp;
	float x;
	float y;
};

PACKET(sc_info)
{
	NetID netid;
	uint8 hp;
	float x;
	float y;
};

PACKET(sc_new_charactor)
{
	NetID netid;
	uint8 hp;
	float x;
	float y;
};

//=============== MOVE_INPUT =================

// 이동연잔자들, 비트연산으로 press,unpress 설정
enum MOVE_DIR : int8
{
	FORWARD = 1 << 0,
	BACK = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3
};

PACKET(cs_test_move)
{
	int8 dir{};
};

//===============  test CHATTING =================

PACKET(cs_test_chat)
{
	char chat[30];
	char padding = '\0';
};

PACKET(sc_test_chat)
{
	NetID chatter_id;
	char chat[30];
	char padding = '\0';
};

//===============  test heart_bit =================
#include <chrono>

PACKET(sc_heart_beat)
{
	
};
PACKET(cs_heart_beat)
{

};
PACKET(sc_disconnect) {
	NetID DisconnectID;
};

#pragma pack(pop)
