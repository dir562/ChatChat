#pragma once
class CObj;
class CProcessServer
{
	SINGLE(CProcessServer)
private:
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;
	int retval;
	HANDLE hMultiThread;

//	std::thread t=
public:
	void ErrorQuit(const char* msg);
	void ErrorDisplay(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);

	int Init();
	void ProcessServer(LPVOID arg);
	
};

