#pragma once



class SocketUtil
{
public:
	constexpr static int SOCKET_SUCCESS = NO_ERROR;

	static void DisplayError(int err, std::wostream& wos = std::wcerr);
	static void terminate();
	static void CheckError(int ret_val);
	static void CheckError(const SOCKET& socket);
	static void ReportErrorWhere(const char* file_name, int line, auto func, std::string_view msg = "", std::ostream& os = std::cerr)
	{
		os << "[ERROR] " << file_name << " line::" << line << " F::" << func 
			<< " err::" << WSAGetLastError() << " msg::" << msg << std::endl;
	}

	static void ReportError(std::string_view msg, std::ostream& os = std::cerr)
	{
		os << "[ERROR] " << msg << " " << WSAGetLastError() << std::endl;
		//SocketUtil::err_display(SocketUtil::GetLastError());
	}
	static int GetLastError() { return ::GetLastError(); }

public:
	static uint32 caculate_bytes_form_WSABUFs(LPWSABUF buf, uint32 count_of_bufs);


public:
};

#define REPORT_ERROR(msg) SocketUtil::ReportErrorWhere(__FILE__, __LINE__, __FUNCTION__, msg);

//////////////////////////////////////

