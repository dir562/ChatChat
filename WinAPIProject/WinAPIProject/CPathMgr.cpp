#include "stdafx.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_szResPath[255] = {};



void CPathMgr::init()
{
	GetCurrentDirectory(255, g_szResPath);
	size_t iLen = wcslen(g_szResPath);

	for (size_t i = iLen - 1; i >= 0; --i)
	{
		if (L'\\' == g_szResPath[i])
		{
			g_szResPath[i] = 0;
			break;
		}		
	}

	wcscat_s(g_szResPath, L"\\bin\\content\\");
}