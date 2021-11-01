#pragma once

#include "IocpHelper.h"

class IocpThread
{
public:
	IocpThread(HANDLE iocp) :iocp_{ iocp } {}

public:
	void ProcessQueuedCompleteOperationLoop();

private:
	void OnRecvComplete(int netid, DWORD returned_bytes, EXP_OVER* ex_over);
	void OnSendComplete(int netid, DWORD returned_bytes, EXP_OVER* ex_over);
	void OnAcceptComplete(EXP_OVER* ex_over);

private:
	HANDLE iocp_;
	// unodered map sessions&
};

