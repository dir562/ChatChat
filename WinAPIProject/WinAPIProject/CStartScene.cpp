#include "stdafx.h"
#include "CStartScene.h"

#include "CPlayer.h"
#include "CTestPlayer.h"
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
	CPlayer* player=dynamic_cast<CPlayer*>(pObj);
	player->SetStartPos(player->GetPos());
	AddObj(pObj, OBJ_TYPE::PLAYER);	

	CTestPlayer* pObj2 = new CTestPlayer;
	pObj2->init();
	pObj2->SetPos(Vec2((float)WIN_X / 2.f+50, (float)WIN_Y / 2.f + 200.f));
	pObj2->SetScale(Vec2(50.f, 50.f));
	AddObj(pObj2, OBJ_TYPE::OTHERPLAYER);

	// �浹 �׷� ����
	CCollisionMgr::GetInst()->CollisionCheckClear();
	CCollisionMgr::GetInst()->CollisionCheck(OBJ_TYPE::PLAYER, OBJ_TYPE::OTHERPLAYER);

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
