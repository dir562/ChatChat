#include "stdafx.h"
#include "CStage01Scene.h"

#include "CKeyMgr.h"

CStage01Scene::CStage01Scene()
{
}

CStage01Scene::~CStage01Scene()
{
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
}

void CStage01Scene::Exit()
{
}
