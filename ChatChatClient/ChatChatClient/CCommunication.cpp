#include "stdafx.h"
#include "CCommunication.h"

CCommunication::CCommunication()
{
}

CCommunication::~CCommunication()
{
}

void CCommunication::ProcessMoveData(UCHAR _Data)
{
	switch (_Data)
	{
	case'W':
	case'w':
		Dir = MOVE_DIR::FORWARD;
		break;

	case 'a':
	case 'A':
		Dir = MOVE_DIR::LEFT;
		break;
	case 'd':
	case 'D':
		Dir = MOVE_DIR::RIGHT;
		break;
	case 'S':
	case's':
		Dir = MOVE_DIR::BACK;
		break;

	}
}
