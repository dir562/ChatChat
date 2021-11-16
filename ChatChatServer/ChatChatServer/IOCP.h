#pragma once

#include "IocpHelper.h"
#include "Session.h"

using Sessions = array<Session, MAX_PLAYER>;

class IOCP
{
public:
	SINGLE_TON(IOCP);

public:
	void ProcessQueuedCompleteOperationLoop();
	void RepeatSendLoop(milliseconds repeat_time = 50ms);

private:
	void OnRecvComplete(NetID net_id, DWORD returned_bytes, EXP_OVER* ex_over);
	void OnSendComplete(NetID net_id, DWORD returned_bytes, EXP_OVER* ex_over);
	void OnAcceptComplete(EXP_OVER* ex_over);
	void OnDisconnectComplete(SOCKET s, NetID net_id, EXP_OVER* ex_over);

	NetID get_new_net_id();


public:
	GET_REF_UNSAFE(sessions);
	GET(iocp);
private:
	HANDLE iocp_;
	Sessions sessions_;
};

