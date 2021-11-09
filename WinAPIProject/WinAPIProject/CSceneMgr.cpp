#include "stdafx.h"
#include "CSceneMgr.h"
#include "CStartScene.h"
#include "CStage01Scene.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
{
	
}

CSceneMgr::~CSceneMgr()
{
	for (int i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneMgr::init()
{
	m_arrScene[(UINT)SCENE_TYPE::START] = new CStartScene;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CStage01Scene;
	
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter();
}

void CSceneMgr::update()
{
	if (nullptr != m_pCurScene)
	{
		m_pCurScene->update();
		m_pCurScene->lateupdate();
		m_pCurScene->finalupdate();
	}	
}

void CSceneMgr::render(HDC _dc)
{
	if (nullptr != m_pCurScene)
	{
		m_pCurScene->render(_dc);
	}	
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNextScene)
{
	// 동일한 Scene 이면 무시
	if (m_pCurScene == m_arrScene[(UINT)_eNextScene])
		return;

	m_pCurScene->Exit();

	m_pCurScene = m_arrScene[(UINT)_eNextScene];

	m_pCurScene->Enter();
}


