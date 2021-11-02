#pragma once


// 전역함수
class CObj;

void CreateObject(CObj* _pNewObj, Vec2 _vPos, OBJ_TYPE _eType);

void DeleteObject(CObj* _pDeleteObj);

void ChangeScene(SCENE_TYPE _eNextScene);

void SaveWString(const wstring& _str, FILE* _pFile);
void LoadWString(wstring& _str, FILE* _pFile);