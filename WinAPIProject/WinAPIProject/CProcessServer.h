#pragma once
class CProcessServer
{
	SINGLE(CProcessServer)
private:
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;
	int retval;
	HANDLE hMultiThread;



public:
	void ErrorQuit(char* msg);
	void ErrorDisplay(char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);

	int Init();

};

