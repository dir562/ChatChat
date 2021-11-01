#pragma once

#include "IocpHelper.h"

class IocpThread
{
public:
	IocpThread(HANDLE iocp) :iocp_{ iocp } {}

public:
	void DoQueuedCompletionStatus();
	void ProcessRecv(int netid, EXP_OVER* ex_over);
	void ProcessSend(int netid, EXP_OVER* ex_over);
	void ProcessAccept(EXP_OVER* ex_over);

private:
	HANDLE iocp_;
	// unodered map sessions&
};

