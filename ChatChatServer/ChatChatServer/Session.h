#pragma once

#include "IocpHelper.h"


// 技记阑 list 趣篮 vector 肺 包府?
// 技记阑 ppl::unodered_map 栏肺 包府?

enum SESSION_STATE : int8
{
	ST_FREE, ST_ACCEPT, ST_INGAME
};

class Session
{
	friend class IOCP;
public:
	Session() = default;
	~Session() { do_disconnect(); }

public:
	void NewSession(SOCKET socket, NetID net_id);

private:
	void clear();
	void HandleIoError(int res);

public:
	void do_recv();
	void do_send(void* packet, size_t packet_len);
	void do_disconnect();

public:
	GET(net_id);
	GET(prerecv_size);
	GET_REF(recv_over);
	GET_REF_UNSAFE(recv_over);

	SET(prerecv_size);
	SET(state);

private:
	EXP_OVER recv_over_{ COMP_OP::OP_RECV };
	DWORD	prerecv_size_{ 0 };
	SOCKET	socket_{ INVALID_SOCKET };
	Spinlock connection_lock_;
	atomic<SESSION_STATE> state_{ SESSION_STATE::ST_FREE }; // is lock free.

	NetID	net_id_{ 0 };
};