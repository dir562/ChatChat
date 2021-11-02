#pragma once

#include "CScene.h"

class CStartScene
	: public CScene
{
public:
	virtual void init();
	virtual void update();

	virtual void Enter();
	virtual void Exit();
};

