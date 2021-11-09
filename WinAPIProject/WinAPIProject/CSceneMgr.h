#pragma once


class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);
private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	CScene* m_pCurScene;

public:
	void init();
	void update();
	void render(HDC _dc);

private:
	void ChangeScene(SCENE_TYPE _eNextScene);
	
	
public:
	CScene* GetCurScene() { return m_pCurScene; }


	friend class CEventMgr;
};

