#include "stdafx.h"
#include "CPlayer.h"

#include "CTexture.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CResMgr.h"
#include "CTexture.h"


#include "CEventMgr.h"

CPlayer::CPlayer()
	: m_pTex(nullptr)
	, m_iHP(10)
	, m_eDir(DIR::DOWN)
	, m_eState(PLAYER_STATE::IDLE)
	, m_ePrevDir(DIR::NONE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_fMoveSpeed(100.f)
	,m_Color(CreateSolidBrush(RGB(255,0,0)))
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::init()
{

	// 충돌체 생성
	CCollider* pCollider = new CCollider;
	pCollider->SetOffsetPos(Vec2(0.f, 10.f));
	pCollider->SetScale(Vec2(30.f, 30.f));
	AddCollider(pCollider);


}

void CPlayer::update()
{

	CheckState();
	Move();
}

void CPlayer::render(HDC _dc)
{
	// 충돌체 랜더링
	if (nullptr != GetCollider())
		GetCollider()->render(_dc);

	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();
	RECT rect = {
		(int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f)
		, (int)(vPos.y + vScale.y / 2.f)
	};
	FillRect(_dc
		,&rect,m_Color);
}


void CPlayer::CheckState()
{	
	// 현재상태를 이전상태로 저장해둠
	m_ePrevState = m_eState;
	m_ePrevDir = m_eDir;

	if (PLAYER_STATE::IDLE == m_eState || PLAYER_STATE::MOVE == m_eState)
	{
		if (KEY_TAP(KEY_TYPE::KEY_LEFT))
		{		
			m_eState = PLAYER_STATE::MOVE;
			m_eDir = DIR::LEFT;
		}
		if (KEY_TAP(KEY_TYPE::SPACE))
		{
		
		}
		if (KEY_TAP(KEY_TYPE::KEY_RIGHT))
		{
			m_eState = PLAYER_STATE::MOVE;
			m_eDir = DIR::DOWN;
		}

		if (KEY_NONE(KEY_TYPE::KEY_W) && KEY_NONE(KEY_TYPE::KEY_S) && KEY_NONE(KEY_TYPE::KEY_A) && KEY_NONE(KEY_TYPE::KEY_D))
		{
			m_eState = PLAYER_STATE::IDLE;			
		}
	}
}


void CPlayer::Move()
{
	if (PLAYER_STATE::MOVE != m_eState)
		return;

	Vec2 vPos = GetPos();

	switch (m_eDir)
	{
	case DIR::UP:
		AddForce(Vec2(0,-1000.f));
		break;
	case DIR::DOWN:
		vPos.y += fDT * m_fMoveSpeed;
		break;
	case DIR::LEFT:
		AddForce(Vec2(-200.f, 0));
		break;
	case DIR::RIGHT:
		AddForce(Vec2(200.f, 0));
		break;	
	}

	SetPos(vPos);
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	int a = 0;
}

