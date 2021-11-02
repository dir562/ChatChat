#pragma once

class CPathMgr
{
private:
	static wchar_t g_szResPath[255];

public:
	static void init();

	static wchar_t* GetResPath()
	{
		return g_szResPath;
	}

private:
	CPathMgr()
	{}
	~CPathMgr()
	{}
};

