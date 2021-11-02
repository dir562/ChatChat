#include "stdafx.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
    for (auto& pair: m_mapTex)
    {
        if (nullptr != pair.second)
            delete pair.second;
    }
}

CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
    assert(nullptr == FindTexture(_strKey));    

    CTexture* pTex = new CTexture;

    wstring strFilePath = CPathMgr::GetResPath();
    strFilePath += _strRelativePath;
    pTex->Load(strFilePath);
    pTex->SetKey(_strKey);
    pTex->SetRelativePath(_strRelativePath);

    // map ¿¡ µî·Ï
    m_mapTex.insert(make_pair(_strKey, pTex));

    return pTex;
}

CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
    map<wstring, CTexture*>::iterator iter = m_mapTex.find(_strKey);

    if (iter == m_mapTex.end())
        return nullptr;

    return iter->second;
}
