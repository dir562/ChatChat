#pragma once
#include "CObj.h"

class CTexture;

enum class TESTPLAYER_STATE
{
	IDLE,
	MOVE,	
	ATTACK,
	DEAD,
};



class CTestPlayer :
	public CObj
{
private:
	bool m_bJump;
	Vec2 m_vStartPos;
	CTexture*		m_pTex;	
	UINT			m_iHP;

	DIR				m_eDir;
	TESTPLAYER_STATE	m_eState;

	DIR				m_ePrevDir;
	TESTPLAYER_STATE	m_ePrevState;

	float			m_fMoveSpeed;

	HBRUSH m_Color;

	float m_fJumpPower;
	int m_iLife;
	vector<COLORREF> m_BrushColor;

public:
	virtual void init();
	float GetJumpPower() { return m_fJumpPower; }
	virtual void update();	
	virtual void render(HDC _dc);
	void SetStartPos(Vec2 _Pos) { m_vStartPos = _Pos; }
	virtual void OnCollisionEnter(CCollider* _pOther);

	void SetLife(int _Life) { m_iLife = _Life; }
	int GetLife() { return m_iLife; }
	virtual CTestPlayer* Clone() { return new CTestPlayer(*this); }


	void SetJumpPower(float _Power) { m_fJumpPower = _Power; }
private:


public:
	CTestPlayer();
	~CTestPlayer();
};

