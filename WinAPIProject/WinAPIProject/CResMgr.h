#pragma once

class CTexture;
class CSound;

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CTexture*>	m_mapTex;
	map<wstring, CSound*>   m_mapSound;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* FindTexture(const wstring& _strKey);
};

