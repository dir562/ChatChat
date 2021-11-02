#pragma once
#include "CObj.h"

class CTexture;

enum class PLAYER_STATE
{
	IDLE,
	MOVE,	
	JUMP,
	ATTACK,
	DEAD,
};



class CPlayer :
	public CObj
{
private:
	CTexture*		m_pTex;	
	UINT			m_iHP;

	DIR				m_eDir;
	PLAYER_STATE	m_eState;

	DIR				m_ePrevDir;
	PLAYER_STATE	m_ePrevState;

	float			m_fMoveSpeed;

	bool			m_bGround;
	HBRUSH m_Color;

public:
	virtual void init();
	virtual void update();	
	virtual void render(HDC _dc);

	virtual void OnCollisionEnter(CCollider* _pOther);

	virtual CPlayer* Clone() { return new CPlayer(*this); }

private:
	void CheckState();
	void CheckAnimation();
	void Move();

public:
	CPlayer();
	~CPlayer();
};

