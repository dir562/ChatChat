#include "stdafx.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObj.h"
#include "CCollider.h"

union myunion
{
	UINT iID;

	struct
	{
		WORD w;
		WORD l;
	};

	UINT iID2;
	UINT iID3;
};


CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJ_TYPE::END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)OBJ_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroup((OBJ_TYPE)iRow, (OBJ_TYPE)iCol);				
			}
		}
	}

}

void CCollisionMgr::CollisionGroup(OBJ_TYPE _eLeft, OBJ_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<CObj*>& vecLeft = pCurScene->GetObjects(_eLeft);
	const vector<CObj*>& vecRight = pCurScene->GetObjects(_eRight);

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->GetCollider())
			{
				continue;
			}

			COLLIDER_PAIR pairid;
			pairid.iLeftID = vecLeft[i]->GetCollider()->GetColliderID();
			pairid.iRightID = vecRight[j]->GetCollider()->GetColliderID();

			map<LONGLONG, bool>::iterator iter = m_mapColPairInfo.find(pairid.iPairID);
			if (m_mapColPairInfo.end() == iter)
			{
				m_mapColPairInfo.insert(make_pair(pairid.iPairID, false));
				iter = m_mapColPairInfo.find(pairid.iPairID);
			}

			if(Collision(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider()))
			{
				// 충돌 했다.
				if (iter->second) // 이전에도 충돌했었다.
				{					
					// 두 오브젝트중 하나라도 삭제 예정상태라면
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						vecLeft[i]->GetCollider()->OnCollisionExit(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollisionExit(vecLeft[i]->GetCollider());
					}
					else
					{
						vecLeft[i]->GetCollider()->OnCollision(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollision(vecLeft[i]->GetCollider());
					}					
				}
				else // 이전에는 충돌하지 않았다.
				{
					// 두 오브젝트중 하나라도 삭제 예정상태라면
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())					
						continue;

					vecLeft[i]->GetCollider()->OnCollisionEnter(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->OnCollisionEnter(vecLeft[i]->GetCollider());
					iter->second = true;
				}
			}
			else // 충돌하지 않는다.
			{				
				if (iter->second) // 이전엔 충돌 했었다.
				{
					vecLeft[i]->GetCollider()->OnCollisionExit(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->OnCollisionExit(vecLeft[i]->GetCollider());
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::Collision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vLeftPos.x - vRightPos.x) <= ((vLeftScale.x + vRightScale.x) / 2.f)
		&& abs(vLeftPos.y - vRightPos.y) <= ((vLeftScale.y + vRightScale.y) / 2.f))
	{
		return true;
	}

	return false;
}


void CCollisionMgr::CollisionCheckClear()
{
	memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJ_TYPE::END);
}

void CCollisionMgr::CollisionCheck(OBJ_TYPE _eLeft, OBJ_TYPE _eRight)
{
	int iRow = -1;
	int iCol = -1;

	if ((UINT)_eLeft < (UINT)_eRight)
	{
		iRow = (UINT)_eLeft;
		iCol = (UINT)_eRight;
	}
	else
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= 1 << iCol;
	}
}
