#pragma once

#include "IocpHelper.h"

class ListenSocket
{
	SINGLE_TON(ListenSocket);

	~ListenSocket();

public:
	void do_accept();

	GET(iocp);


private:
	SOCKET		listen_socket_{};
	SOCKET		newface_socket_{};
	HANDLE		iocp_{};
	EXP_OVER	accept_ex_{};
	char		accept_buf_[sizeof(SOCKADDR_IN) * 2 + 32 + 100]{};
};