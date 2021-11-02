#include "stdafx.h"
#include "CCollider.h"

#include "CObj.h"
#include "CCore.h"
#include "CCamera.h"

UINT CCollider::g_iColliderNextID = 0;

CCollider::CCollider()
	: m_pObject(nullptr)
	, m_vScale()
	, m_vOffsetPos()
	, m_vFinalPos()	
	, m_iCollisionCount(0)
	, m_iColliderID(g_iColliderNextID++)
	, m_eType(COLLIDER_TYPE::RECT)
	, m_ePen(PEN_TYPE::GREEN)
{
}

CCollider::CCollider(const CCollider& _Origin)
	: m_pObject				(nullptr)
	, m_vScale				(_Origin.m_vScale)
	, m_vOffsetPos			(_Origin.m_vOffsetPos)
	, m_vFinalPos			(_Origin.m_vFinalPos)
	, m_iCollisionCount		(0)
	, m_eType				(_Origin.m_eType)
	, m_ePen				(_Origin.m_ePen)
	, m_iColliderID			(g_iColliderNextID++)
{
}

CCollider::~CCollider()
{

}

void CCollider::finalupdate()
{
	assert(!(m_iCollisionCount < 0));

	m_vFinalPos = m_pObject->GetPos() + m_vOffsetPos;
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCollisionCount;
	if(0 < m_iCollisionCount)
		m_ePen = PEN_TYPE::RED;

	m_pObject->OnCollisionEnter(_pOther);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pObject->OnCollision(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCollisionCount;
	if(m_iCollisionCount == 0)
		m_ePen = PEN_TYPE::GREEN;

	m_pObject->OnCollisionExit(_pOther);
}

void CCollider::render(HDC _dc)
{
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(_dc, GetStockObject(HOLLOW_BRUSH));
	HPEN hPen = CCore::GetInst()->GetPen(m_ePen);
	HPEN hPrevPen = (HPEN)SelectObject(_dc, hPen);


	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	Rectangle(_dc, (int)(vRenderPos.x - m_vScale.x / 2.f)
	, (int)(vRenderPos.y - m_vScale.y / 2.f)
	, (int)(vRenderPos.x + m_vScale.x / 2.f)
	, (int)(vRenderPos.y + m_vScale.y / 2.f));

	SelectObject(_dc, hPrevBrush);
	SelectObject(_dc, hPrevPen);
}