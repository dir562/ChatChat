#pragma once
#include "CObj.h"

class CTexture;

enum class PLAYER_STATE
{
	IDLE,
	MOVE,	
	ATTACK,
	DEAD,
};



class CPlayer :
	public CObj
{
private:
	bool m_bJump;
	Vec2 m_vStartPos;
	CTexture*		m_pTex;	
	UINT			m_iHP;

	DIR				m_eDir;
	PLAYER_STATE	m_eState;

	DIR				m_ePrevDir;
	PLAYER_STATE	m_ePrevState;

	float			m_fMoveSpeed;

	HBRUSH m_Color;

	float m_fJumpPower;
	

public:
	virtual void init();

	virtual void update();	
	virtual void render(HDC _dc);
	void SetStartPos(Vec2 _Pos) { m_vStartPos = _Pos; }
	virtual void OnCollisionEnter(CCollider* _pOther);

	
	virtual CPlayer* Clone() { return new CPlayer(*this); }

private:
	void CheckState();
	void Jumping();	
	void Move();
	void ValueInit();

public:
	CPlayer();
	~CPlayer();
};

