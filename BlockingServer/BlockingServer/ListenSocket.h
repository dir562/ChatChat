#pragma once

class ListenSocket
{
	SINGLE_TON(ListenSocket);

public:
	SOCKET accept();

private:
	~ListenSocket();
	
private:
	SOCKADDR_IN server_addr;
	SOCKET listen_socket_;
};

