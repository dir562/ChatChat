#pragma once


class CRes
{
private:
	wstring			m_strKey;
	wstring			m_strRelativePath;

private:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }

public:
	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }

public:
	CRes();
	virtual ~CRes();

	friend class CResMgr;
};

