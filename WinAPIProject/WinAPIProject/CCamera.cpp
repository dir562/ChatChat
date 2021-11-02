#include "stdafx.h"
#include "CCamera.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCamera::CCamera()
	: m_vLookAt(0.f, 0.f)
	, m_fCamSpeed(500.f)	
{
}

CCamera::~CCamera()
{
	DeleteObject(m_hVeil);
	DeleteDC(m_memDC);
}

void CCamera::init()
{
	m_vLookAt = Vec2((float)WIN_X / 2.f, (float)WIN_Y / 2.f);

	// Veil Texture »ý¼º
	m_hVeil = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), WIN_X, WIN_Y);
	m_memDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_memDC, m_hVeil);
	DeleteObject(hOldBitmap);
}

void CCamera::update()
{
	
	m_vDiff = m_vLookAt - Vec2((float)WIN_X / 2.f, (float)WIN_Y / 2.f);

	ExcuteFX();
}

void CCamera::render(HDC _dc)
{
	if (m_listFX.empty())
		return;

	BLENDFUNCTION ftn = {};

	ftn.BlendOp = AC_SRC_OVER;
	ftn.AlphaFormat = 0;

	int alpha = (int)(255.f * m_fRatio);
	ftn.SourceConstantAlpha = alpha;

	AlphaBlend(_dc, 0, 0, WIN_X, WIN_Y, m_memDC, 0, 0, WIN_X, WIN_Y, ftn);
}

void CCamera::ExcuteFX()
{
	if (m_listFX.empty())
		return;

	m_listFX.front().fCurTime += fDT;
	
	if (m_listFX.front().fTime < m_listFX.front().fCurTime)
	{
		m_listFX.pop_front();
	}
	else
	{
		m_fRatio = m_listFX.front().fCurTime / m_listFX.front().fTime;
		if (FX_TYPE::FADE_IN == m_listFX.front().eType)
		{
			m_fRatio = 1.f - m_fRatio;
		}
	}	
}