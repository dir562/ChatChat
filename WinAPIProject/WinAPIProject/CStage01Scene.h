#pragma once
#include "CScene.h"

class CStage01Scene :
	public CScene
{

public:
	virtual void update();

	virtual void Enter();
	virtual void Exit();

public:
	CStage01Scene();
	~CStage01Scene();
};

