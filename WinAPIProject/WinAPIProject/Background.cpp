#include "stdafx.h"
#include "Background.h"
#include "CResMgr.h"
#include "CTexture.h"

void CBackground::Init()
{
	m_pTex = CResMgr::GetInst()->FindTexture(L"Background");
	if(nullptr==m_pTex)
		m_pTex= CResMgr::GetInst()->LoadTexture(L"Background", L"BackGround.bmp");
	Vec2 vPos = GetPos();
	vPos.x += WIN_X/2;
	vPos.y += WIN_Y / 2;
	SetPos(vPos);

}

void CBackground::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	float fWidth=m_pTex->Width();
	float fHeight = m_pTex->Height();
	BitBlt(_dc, (int)(vPos.x - fWidth / 2.f)
		, (int)(vPos.y - fHeight / 2.f)
		, (int)fWidth, (int)fHeight
		, m_pTex->GetDC()
		, 0, 0, SRCCOPY);
}
