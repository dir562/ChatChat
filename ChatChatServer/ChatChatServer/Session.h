#pragma once

#include "IocpHelper.h"


// 技记阑 list 趣篮 vector 肺 包府?
// 技记阑 ppl::unodered_map 栏肺 包府?

BETTER_ENUM(SESSION_STATE, int8, ST_FREE, ST_ACCEPT, ST_INGAME);

class Session
{
public:
	Session(SOCKET socket, uint8 net_id) :socket_{ socket }, net_id_{ net_id }{}
	~Session() { do_disconnect(); }

public:
	void do_recv();
	void do_send(void* packet, size_t packet_len);
	void do_disconnect();

	GET(net_id);
	GET(prerecv_size);
	GET_REF(recv_over);
	GET_REF_UNSAFE(recv_over);
	GET(state);

	SET(prerecv_size);
	SET(state);

private:
	uint8	net_id_{ 0 };
	SOCKET	socket_{ 0 };
	uint32	prerecv_size_{ 0 };
	EXP_OVER recv_over_{ COMP_OP::OP_RECV };
	SESSION_STATE state_{ SESSION_STATE::ST_FREE };
};