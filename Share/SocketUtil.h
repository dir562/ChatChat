#pragma once



class SocketUtil
{
public:
	constexpr static int SOCKET_SUCCESS = NO_ERROR;

	static void err_display(int err, std::wostream& wos = std::wcerr);
	static void terminate();
	static void CheckError(int ret_val);
	static void CheckError(const SOCKET& socket);
	static void ReportError(std::string_view msg, std::ostream& os = std::cerr) 
	{ 
		os << "[ERROR] " << msg << " " << WSAGetLastError() << std::endl; 
		SocketUtil::err_display(SocketUtil::GetLastError());
	}
	static int GetLastError() { return ::GetLastError(); }

public:
	static uint32 caculate_bytes_form_WSABUFs(LPWSABUF buf, uint32 count_of_bufs);


public:
};

//////////////////////////////////////

