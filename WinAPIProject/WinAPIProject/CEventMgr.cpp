#include "stdafx.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObj.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// ======================
	// 삭제 예정 오브젝트 삭제
	// ======================
	for (size_t i = 0; i < m_vecDeadObj.size(); ++i)
	{
		if (nullptr != m_vecDeadObj[i])
			delete m_vecDeadObj[i];
	}
	m_vecDeadObj.clear();


	// ==========
	// Event 처리
	// ==========
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		ExcuteEvent(m_vecEvent[i]);
	}
	// 이벤트 클리어
	m_vecEvent.clear();
}

void CEventMgr::ExcuteEvent(const tEvent& _event)
{
	switch (_event.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// wParam : Object 주소
		// lParam : 그룹 타입
		CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
		pScene->AddObj((CObj*)_event.wParam, (OBJ_TYPE)_event.lParam);
	}

	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// wParam : 삭제 될 오브젝트 주소
		((CObj*)_event.wParam)->SetDead();
		m_vecDeadObj.push_back(((CObj*)_event.wParam));
	}

	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// wParam : 다음 SceneType		
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_event.wParam);
	}
	break;
	default:
		break;
	}
}
#include "CPlayer.h"

void CEventMgr::CallCreatePlayer(int netid, int hp, float x, float y)
{
	tEvent Type;
	Type.eEvent = EVENT_TYPE::CREATE_OBJECT;
	CPlayer* pObj = new CPlayer();
	pObj->SetID(netid);
	pObj->SetLife(hp);
	pObj->SetPos(Vec2(x, y));
	pObj->init();

	pObj->SetScale(Vec2(50.f, 50.f));
	Type.wParam = (DWORD_PTR)pObj;
	Type.lParam = (DWORD_PTR)OBJ_TYPE::PLAYER;
	AddEvent(Type);

}
#include "CTestPlayer.h"
void CEventMgr::CallCreateOtherPlayer(int netid, int hp, float x, float y)
{

	tEvent Type;
	Type.eEvent = EVENT_TYPE::CREATE_OBJECT;
	CTestPlayer* pObj = new CTestPlayer();
	pObj->SetID(netid);
	pObj->SetLife(hp);
	pObj->SetPos(Vec2(x, y));
	pObj->init();

	pObj->SetScale(Vec2(50.f, 50.f));
	Type.wParam = (DWORD_PTR)pObj;
	Type.lParam = (DWORD_PTR)OBJ_TYPE::OTHERPLAYER;
	AddEvent(Type);
}
