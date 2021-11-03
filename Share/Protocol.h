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
	
	, CS_NONE			= 10
	, CS_TEST_CHAT



	/* Server 2 Client */

	, SC_NONE			= 100
	, SC_TEST_CHAT
	, SC_TEST_HEART_BIT




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


//=============== MOVE_INPUT =================

// 이동연잔자들, 비트연산으로 press,unpress 설정
enum class MOVE_DIR : int8
{
	FORWARD = 1 << 0,
	BACK = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3
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

PACKET(sc_test_heart_bit)
{
	std::chrono::milliseconds time_after_send;
};


#pragma pack(pop)
