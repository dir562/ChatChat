#include "stdafx.h"
#include "CScene.h"
#include "CObj.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (nullptr != m_arrObj[i][j])
				delete m_arrObj[i][j];
		}
	}
}

void CScene::update()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if(!m_arrObj[i][j]->IsDead())
				m_arrObj[i][j]->update();
		}
	}	
}

void CScene::lateupdate()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
				m_arrObj[i][j]->lateupdate();
		}
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
				m_arrObj[i][j]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		vector<CObj*>& vecObj = m_arrObj[i];
		vector<CObj*>::iterator iter = vecObj.begin();

		for (; iter != vecObj.end(); )
		{
			if ((*iter)->IsDead())
			{
				iter = vecObj.erase(iter);
			}
			else
			{
				(*iter)->render(_dc);
				++iter;
			}
		}
	}
}

void CScene::DeleteGroup(OBJ_TYPE _eType)
{
	vector<CObj*>& vecObj = m_arrObj[(UINT)_eType];

	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		assert(vecObj[i]);
		delete vecObj[i];
	}

	vecObj.clear();
}

void CScene::DeleteAllObject()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		DeleteGroup((OBJ_TYPE)i);
	}
}

void CScene::AddObj(CObj* _pObj, OBJ_TYPE _eType)
{

	m_arrObj[(UINT)_eType].push_back(_pObj);
}
