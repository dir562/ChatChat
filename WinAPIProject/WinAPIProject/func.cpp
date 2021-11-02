#include "stdafx.h"
#include "func.h"

#include "CObj.h"
#include "CEventMgr.h"

void CreateObject(CObj* _pNewObj, Vec2 _vPos, OBJ_TYPE _eType)
{
	_pNewObj->SetPos(_vPos);

	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_eType;
	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(CObj* _pDeleteObj)
{
	// »èÁ¦
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_pDeleteObj;
	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNextScene)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.wParam = (DWORD_PTR)_eNextScene;
	CEventMgr::GetInst()->AddEvent(evn);
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	BYTE length = _str.length();
	const wchar_t* pStr = _str.c_str();

	fwrite(&length, sizeof(BYTE), 1, _pFile);
	fwrite(pStr, sizeof(wchar_t), length, _pFile);
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	BYTE length = 0;	
	fread(&length, sizeof(BYTE), 1, _pFile);

	wchar_t szBuffer[256] = {};
	fread(szBuffer, sizeof(wchar_t), (size_t)length, _pFile);
	_str = szBuffer;
}
