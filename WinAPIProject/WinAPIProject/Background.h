#pragma once
#include "CObj.h"
class CBackground :
    public CObj
{
private:
    CTexture* m_pTex;
public:
    void Init();
    void render(HDC _dc);
    virtual CBackground* Clone() { return new CBackground(*this); }
};

