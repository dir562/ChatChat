#pragma once

#define SINGLE(type) public: \
						static type* GetInst()\
						{\
							static type inst;\
							return &inst;\
						}\
					private:\
						type();\
						~type();


#define KEY_CHECK(key, state) state == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

#define fDT CTimeMgr::GetInst()->GetfDT()

#define WIN_X 1280
#define WIN_Y 768

#define GRAVITY 2400

#define BUFSIZE 512
#define SERVERPORT 9000


enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,



	END,
};

enum class OBJ_TYPE
{
	DEFAULT,	
	PLAYER,
	OTHERPLAYER,

	END = 32,
};

enum class PEN_TYPE
{
	GREEN,
	RED,
	BLUE,
	END,
};

enum class COLLIDER_TYPE
{
	RECT,
	CIRCLE,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	END,
};

enum class DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE,
};

struct Move_Packet {
	float X;
	float Y;
	int ClientID;
};
struct Disconnet_Packet {
	int GetClientID;
};


