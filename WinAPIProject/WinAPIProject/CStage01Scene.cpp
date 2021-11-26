#include "stdafx.h"
#include "CStage01Scene.h"

#include "CKeyMgr.h"
#include "CCamera.h"
#include "Background.h"
CStage01Scene::CStage01Scene()
{

}

CStage01Scene::~CStage01Scene()
{
}

void CStage01Scene::init()
{
	CBackground* Ground = new CBackground();
	Ground->Init(L"Title",L"Title.bmp");
	AddObj(Ground, OBJ_TYPE::DEFAULT);
	CCamera::GetInst()->init();
}

void CStage01Scene::update()
{
	CScene::update();
	if (KEY_TAP(KEY_TYPE::ENTER))
	{
		ChangeScene(SCENE_TYPE::START);
	}
}

void CStage01Scene::Enter()
{
	init();
}

void CStage01Scene::Exit()
{
}
