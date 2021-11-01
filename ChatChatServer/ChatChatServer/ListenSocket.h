#pragma once

#include "IocpHelper.h"

class ListenSocket
{
	SINGLE_TON(ListenSocket) = default;

	~ListenSocket();

public:
	void Init(HANDLE iocp);
	void do_accept();

private:
	SOCKET		listen_socket_{};
	SOCKET		newface_socket_{};
	EXP_OVER	accept_ex_{ COMP_OP::OP_ACCEPT };
	char		accept_buf_[sizeof(SOCKADDR_IN) * 2 + 32 + 100]{};
};