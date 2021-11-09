#include "stdafx.h"
#include "CProcessServer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CObj.h"


void CProcessServer::ErrorQuit(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CProcessServer::ErrorDisplay(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
	exit(1);
}

int CProcessServer::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;
		else if (0 == received)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}
int CProcessServer::Init()
{
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa)) 
		return 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	return 0;
}

void CProcessServer::ProcessServer(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	CObj* pObj=nullptr;
	char buf[BUFSIZE + 1];
	//getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	while (true)
	{
		retval = recvn(client_sock, (char*)pObj, BUFSIZE, 0);
		if (SOCKET_ERROR == retval) {
			ErrorDisplay("recv()");
			break;
		}
		else if (0 == retval) {
			break;
		}

	}
	vector<CObj*> objects=const_cast<vector<CObj*>&>(CSceneMgr::GetInst()->GetCurScene()->GetObjects(OBJ_TYPE::OTHERPLAYER));
	std::vector<CObj*>::iterator it=objects.begin();
	bool bIsFind = false;
	for (; it != objects.end(); ++it) {
		if ((*it)->GetID() == (pObj)->GetID()) {
			bIsFind = true;
			break;
		}
	}

	if(!bIsFind)
		CSceneMgr::GetInst()->GetCurScene()->AddObj(pObj, OBJ_TYPE::OTHERPLAYER);
	

	closesocket(client_sock);

}
