#pragma once

class CCollider;

union COLLIDER_PAIR
{
	struct {
		UINT iLeftID;
		UINT iRightID;
	};
	LONGLONG iPairID;
};


class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	UINT	m_arrCheck[(UINT)OBJ_TYPE::END];

	map<LONGLONG, bool>	m_mapColPairInfo;

public:
	void update();

public:
	void CollisionCheck(OBJ_TYPE _eLeft, OBJ_TYPE _eRight);
	void CollisionCheckClear();

private:
	void CollisionGroup(OBJ_TYPE _eLeft, OBJ_TYPE _eRight);
	bool Collision(CCollider* _pLeftCol, CCollider* _pRightCol);
	
};

