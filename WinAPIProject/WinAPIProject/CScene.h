#pragma once

class CObj;


class CScene
{
private:
	vector<CObj*>	m_arrObj[(UINT)OBJ_TYPE::END];

public:
	virtual void init(){}

	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();
	virtual void render(HDC _dc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

protected:
	void DeleteGroup(OBJ_TYPE _eType);
	void DeleteAllObject();
	

public:
	void AddObj(CObj* _pObj, OBJ_TYPE _eType = OBJ_TYPE::DEFAULT);	

	const vector<CObj*>& GetObjects(OBJ_TYPE _eType) 
	{ 
		return m_arrObj[(UINT)_eType]; 
	}



public:
	CScene();
	virtual ~CScene();
};

