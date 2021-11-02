#include "stdafx.h"
#include "CTexture.h"

#include "CCore.h"

CTexture::CTexture()
	: m_hBit(nullptr)
	, m_hDC(nullptr)
	, m_tInfo{}
{
}

CTexture::~CTexture()
{
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
}

void CTexture::Load(const wstring& _strPath)
{
	m_hBit = (HBITMAP)LoadImage(nullptr, _strPath.c_str()
		, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);			

	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	HBITMAP hPrev = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hPrev);	

	GetObject(m_hBit, sizeof(BITMAP), &m_tInfo);
}