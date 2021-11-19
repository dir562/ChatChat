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
#include "CTestPlayer.h"
#include "CEventMgr.h"

#include "Networker.h"

CPlayer::CPlayer()
	: m_pTex(nullptr)
	, m_iHP(10)
	, m_eDir(DIR::DOWN)
	, m_eState(PLAYER_STATE::IDLE)
	, m_ePrevDir(DIR::NONE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_fMoveSpeed(100.f)
	, m_fJumpPower(1200.f)
	, m_bJump(false)

	, m_iLife(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::init()
{

	// 충돌체 생성
	CCollider* pCollider = new CCollider;
	pCollider->SetOffsetPos(Vec2(0.f, 0.f));
	pCollider->SetScale(Vec2(30.f, 30.f));
	AddCollider(pCollider);

	m_vStartPos = GetPos();
	m_BrushColor.push_back(RGB(255, 0, 0));
	m_BrushColor.push_back(RGB(255, 127, 0));
	m_BrushColor.push_back(RGB(255, 255, 0));
	m_BrushColor.push_back(RGB(0, 255, 0));
	m_BrushColor.push_back(RGB(0, 0, 255));
	m_BrushColor.push_back(RGB(0, 0, 128));
	m_BrushColor.push_back(RGB(112, 93, 168));
	m_BrushColor.push_back(RGB(0, 0, 0));
	m_Color = (CreateSolidBrush(m_BrushColor[m_iLife]));

}

void CPlayer::update()
{

	CheckState();
	Move();
	Jumping();
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
		, &rect, m_Color);
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (7 == m_iLife)
		return;
	CTestPlayer* player = dynamic_cast<CTestPlayer*>(_pOther->GetObj());
	bool  b = false;
	if (player->GetJumpPower() < 0) {
		b = true;
	}
	if (GetPos().y > _pOther->GetObj()->GetPos().y && b) {
		m_iLife += 1;
		m_Color = CreateSolidBrush(m_BrushColor[m_iLife]);
	}

	if (player->GetJumpPower() < 0 && m_fJumpPower > 0) {
		m_fJumpPower = -100.f;
	}

}

void CPlayer::OnCollision(CCollider* _pOther)
{
	if (7 == m_iLife)
		return;
	CTestPlayer* player = dynamic_cast<CTestPlayer*>(_pOther->GetObj());
	bool b = false;
	if (player->GetJumpPower() < 0)
		b = true;
	if (!b)
		m_fJumpPower = 700.f;

}


void CPlayer::PressSpaceBar()
{
	m_iLife = 0;
	m_Color = (CreateSolidBrush(m_BrushColor[m_iLife]));

	Vec2 vPos = GetPos();

	int random = rand() % 200 - 100;
	vPos.x += random;
	SetPos(vPos);

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
			if (7 == m_iLife) {
				PressSpaceBar();
				return;
			}
			m_bJump = true;
		}
		if (KEY_TAP(KEY_TYPE::KEY_RIGHT))
		{
			m_eState = PLAYER_STATE::MOVE;
			m_eDir = DIR::RIGHT;
		}

		if (KEY_NONE(KEY_TYPE::KEY_LEFT) && KEY_NONE(KEY_TYPE::KEY_RIGHT))
		{
			m_eState = PLAYER_STATE::IDLE;
		}
	}
}

void CPlayer::Jumping()
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


void CPlayer::Move()
{
	if (PLAYER_STATE::MOVE != m_eState || 7 == m_iLife)
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

void CPlayer::ValueInit()
{
	Vec2 vPos = GetPos();
	vPos.y = m_vStartPos.y;
	SetPos(vPos);
	m_bJump = false;
	m_fJumpPower = 1200.f;

}


