#include "stdafx.h"
#include "CTestPlayer.h"

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
#include "CPlayer.h"

#include "CEventMgr.h"

CTestPlayer::CTestPlayer()
	: m_pTex(nullptr)
	, m_iHP(10)
	, m_eDir(DIR::DOWN)
	, m_eState(TESTPLAYER_STATE::IDLE)
	, m_ePrevDir(DIR::NONE)
	, m_ePrevState(TESTPLAYER_STATE::IDLE)
	, m_fMoveSpeed(100.f)
	,m_fJumpPower(0.f)
	,m_bJump(false)
	, m_iLife(7)
	
{
}

CTestPlayer::~CTestPlayer()
{
}

void CTestPlayer::init()
{

	// 충돌체 생성
	CCollider* pCollider = new CCollider;
	pCollider->SetOffsetPos(Vec2(0.f, 0.f));
	pCollider->SetScale(Vec2(50.f, 50.f));
	AddCollider(pCollider);

	m_vStartPos = GetPos();
	m_BrushColor.push_back(RGB(0, 0, 0));
	m_BrushColor.push_back(RGB(112, 93, 168));
	m_BrushColor.push_back(RGB(0, 0, 128));
	m_BrushColor.push_back(RGB(0, 0, 255));
	m_BrushColor.push_back(RGB(0, 255, 0));
	m_BrushColor.push_back(RGB(255, 255, 0));
	m_BrushColor.push_back(RGB(255, 127, 0));
	m_BrushColor.push_back(RGB(255, 0, 0));

	m_Color = (CreateSolidBrush(m_BrushColor[m_iLife]));

}

void CTestPlayer::update()
{
//	CheckState();
	Move();
	Jumping();
	
}

void CTestPlayer::render(HDC _dc)
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


void CTestPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (0 == m_iLife)
		return;
	CPlayer* player = dynamic_cast<CPlayer*>(_pOther->GetObj());
	bool  b=false;
	if (player->GetJumpPower() < 0) {
		b = true;
	}


	if (GetPos().y > _pOther->GetObj()->GetPos().y&&b) {
		m_iLife -= 1;
		m_Color = CreateSolidBrush(m_BrushColor[m_iLife]);
	}
}

void CTestPlayer::MovingData(UINT _uKey, int _bPress)
{
	if (0 == _uKey) {
		m_eDir = DIR::NONE;
		m_eState = TESTPLAYER_STATE::IDLE;
	}
	else if (1 == _uKey) {
		if (_bPress) {
			return;
		}
		m_bJump = true;
	}
	else if (2 == _uKey) {
		m_eDir = DIR::LEFT;
		m_eState = TESTPLAYER_STATE::MOVE;
	}
	else if (3 == _uKey) {
		m_eDir = DIR::RIGHT;
		m_eState = TESTPLAYER_STATE::MOVE;
	}
}

void CTestPlayer::CheckState()
{
	m_ePrevState = m_eState;
	m_ePrevDir = m_eDir;

	if (TESTPLAYER_STATE::IDLE == m_eState || TESTPLAYER_STATE::MOVE == m_eState)
	{
		if (KEY_TAP(KEY_TYPE::KEY_LEFT))
		{
			m_eState = TESTPLAYER_STATE::MOVE;
			m_eDir = DIR::LEFT;
		}
		if (KEY_TAP(KEY_TYPE::SPACE))
		{
			if (0 == m_iLife) {
			//	PressSpaceBar();
				return;
			}
			m_bJump = true;
		}
		if (KEY_TAP(KEY_TYPE::KEY_RIGHT))
		{
			m_eState = TESTPLAYER_STATE::MOVE;
			m_eDir = DIR::RIGHT;
		}

		if (KEY_NONE(KEY_TYPE::KEY_LEFT) && KEY_NONE(KEY_TYPE::KEY_RIGHT))
		{
			m_eState = TESTPLAYER_STATE::IDLE;
		}
	}
}

void CTestPlayer::Jumping()
{
	if (m_bJump) {
		Vec2 vPos = GetPos();
		if (vPos.y > m_vStartPos.y) {
			ValueInit();
			return;
		}
		vPos.y -= m_fJumpPower * fDT;
		m_fJumpPower -= GRAVITY * fDT;

		SetPos(vPos);
	}
}

void CTestPlayer::Move()
{
	if (TESTPLAYER_STATE::MOVE != m_eState || 0 == m_iLife)
		return;

	Vec2 vPos = GetPos();

	switch (m_eDir)
	{

	case DIR::LEFT:
	{
		vPos.x -= fDT * m_fMoveSpeed;


	} break;
	case DIR::RIGHT:
	{
		vPos.x += fDT * m_fMoveSpeed;


	}break;
	}

	SetPos(vPos);
}

void CTestPlayer::ValueInit()
{

	Vec2 vPos = GetPos();
	vPos.y = m_vStartPos.y;
	SetPos(vPos);
	m_bJump = false;
	m_fJumpPower = 1200.f;
}

