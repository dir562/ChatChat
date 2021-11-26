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
	bool m_bAttack;

public:
	bool IsAttack(CObj* _Obj1, CObj* _Obj2);
	bool GetAttack() { return m_bAttack; }
	void SetAttack(bool _bAtt) { m_bAttack = _bAtt; }
	virtual void init();
	float GetJumpPower() { return m_fJumpPower; }
	virtual void update();	
	bool GetJumping() { return m_bJump; }
	virtual void render(HDC _dc);
	void SetStartPos(Vec2 _Pos) { m_vStartPos = _Pos; }
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);

	void SetLife(int _Life) { m_iLife = _Life; }
	void SetLifeandColor();
	int GetLife() { return m_iLife; }
	virtual CTestPlayer* Clone() { return new CTestPlayer(*this); }

	void MovingData(UINT _uKey);

	void SetJumpPower(float _Power) { m_fJumpPower = _Power; }
private:
	void CheckState();
	void Jumping();
	void Move();
	void ValueInit();

public:

	CTestPlayer();
	~CTestPlayer();
};

