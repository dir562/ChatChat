#pragma once
#include "CRes.h"

class CTexture :
	public CRes
{
private:
	HBITMAP		m_hBit;
	HDC			m_hDC;
	BITMAP		m_tInfo;

private:
	void Load(const wstring& _strPath);

public:
	HDC GetDC() { return m_hDC; }
	int Width() { return m_tInfo.bmWidth; }
	int Height() { return m_tInfo.bmHeight; }

public:
	CTexture();
	~CTexture();

	friend class CResMgr;
};

