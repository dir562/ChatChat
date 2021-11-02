#include "stdafx.h"
#include "CTimeMgr.h"

#include "CCore.h"

CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llOldCount{}
	, m_llCurCount{}
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	QueryPerformanceFrequency(&m_llFrequency);
	QueryPerformanceCounter(&m_llOldCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dDeltaTime = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_dFPS = 1. / m_dDeltaTime;


	m_llPrevCount = m_llCurCount;


	// 1초마다 Frequency 갱신
	if (m_llCurCount.QuadPart - m_llOldCount.QuadPart > m_llFrequency.QuadPart)
	{
		QueryPerformanceFrequency(&m_llFrequency);
		m_llOldCount = m_llCurCount;
	}
}

void CTimeMgr::render(HDC _dc)
{
	static double dTime = 0.;

	dTime += m_dDeltaTime;

	if (1. < dTime)
	{
		wchar_t szDeltaTime[255] = {};
		swprintf(szDeltaTime, 255, L"FPS : %lf, DT : %lf ", m_dFPS, m_dDeltaTime);
		SetWindowText(CCore::GetInst()->GetHwnd(), szDeltaTime);
		dTime = 0.;
	}	
}