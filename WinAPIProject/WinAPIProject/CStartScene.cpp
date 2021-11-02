#include "stdafx.h"
#include "CStartScene.h"

#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CCamera.h"

#include "CKeyMgr.h"

void CStartScene::init()
{
	// �ӽ� ������Ʈ �ϳ� ����
	CObj* pObj = new CPlayer;
	pObj->init();
	pObj->SetPos(Vec2((float)WIN_X / 2.f, (float)WIN_Y / 2.f + 200.f));
	pObj->SetScale(Vec2(50.f, 50.f));
	AddObj(pObj, OBJ_TYPE::PLAYER);	

	// �浹 �׷� ����
	CCollisionMgr::GetInst()->CollisionCheckClear();
	CCollisionMgr::GetInst()->CollisionCheck(OBJ_TYPE::PLAYER, OBJ_TYPE::MONSTER);

	// ī�޶� �ʱ�ȭ
	CCamera::GetInst()->init();
}

void CStartScene::update()
{
	CScene::update();
}

void CStartScene::Enter()
{
	init();	
}

void CStartScene::Exit()
{
	DeleteAllObject();
}
