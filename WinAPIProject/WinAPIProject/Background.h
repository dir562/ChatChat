#pragma once
#include "CObj.h"
class CBackground :
    public CObj
{
private:
    CTexture* m_pTex;
public:
    void Init();
    void Init(const wchar_t* _pName, const wchar_t* _pPath);
    void render(HDC _dc);
    virtual CBackground* Clone() { return new CBackground(*this); }
};

