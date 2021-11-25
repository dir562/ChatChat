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
const int MAX_PACKET_SIZE = std::numeric_limits<packet_size_t>::max() + 1;

//====================================

constexpr int MAX_HP = 7;
constexpr float DEFAULT_X = 840.f;
constexpr float DEFAULT_Y = 639.5f;

//====================================

#pragma warning(push)
#pragma warning(disable: 26812)

BETTER_ENUM
(
	PACKET_TYPE, int8

	, NONE = 0

	/* Client 2 Server */

	, CS_NONE = 10
	, CS_TEST_CHAT
	, CS_MY_INFO
	, CS_HI
	, CS_HEART_BEAT
	, CS_KEY_INPUT
	, CS_ATTACK

	/* Server 2 Client */

	, SC_NONE = 100
	, SC_TEST_CHAT
	, SC_HEART_BEAT
	, SC_NEW_CHARACTOR
	, SC_HI_OK
	, SC_INFO
	, SC_DISCONNECT
	, SC_KEY_INPUT
	, SC_ATTACK
);

#pragma warning(pop)


#pragma pack (push, 1)
//================ BASE ================


template<class T>
struct packet_base
{
	packet_size_t size = sizeof(T);
	PACKET_TYPE packet_type = +PACKET_TYPE::_from_string_nocase(typeid(T).name() + 7);
};
#define PACKET(name) struct name : packet_base<name>											

PACKET(none)
{
};

//=============== new connection =================

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

//=============== key_input =================

enum KEY_INPUT : int8
{
	NONE = 0,
	SPACE = 1,
	LEFT = 2,
	RIGHT = 3,
};

PACKET(cs_key_input)
{
	KEY_INPUT key;
};

PACKET(sc_key_input)
{
	NetID netid;
	KEY_INPUT key;
};

//=============== attack =================

PACKET(cs_attack)
{
	NetID be_attacked_id;
};

PACKET(sc_attack)
{
	NetID attacker_id;
	NetID be_attacked_id;
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

//===============  heart_bit & disconnect =================

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
