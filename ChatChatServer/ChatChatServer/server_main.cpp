#include "stdafx.h"
#include "Session.h"
#include "IOCP.h"

void InitWsa()
{
	WSADATA WSAData; 
	int res = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (SOCKET_ERROR == res)
	{
		SocketUtil::ReportError("InitWsa FAIL.");
		exit(-1);
	}
}

void EndWsa()
{
	WSACleanup();
}


int main()
{
	InitWsa();

	const auto NUM_IOCP_THREADS = thread::hardware_concurrency() - 1;
	vector<thread> IocpWokers; IocpWokers.reserve(NUM_IOCP_THREADS);
	for (size_t i = 0; i < NUM_IOCP_THREADS; i++)
	{
		IocpWokers.emplace_back(&IOCP::ProcessQueuedCompleteOperationLoop, &IOCP::get());
	}
	cerr << NUM_IOCP_THREADS << " threads are do GQCS work(except main thread)." << endl;

	// ToDo On MainThread.
	//IOCP::get().RepeatSendLoop();
	//


	for (auto& workers : IocpWokers) workers.join();

	EndWsa();
}