#pragma once


class CCommunication
{
	SINGLE(CCommunication);

private:
	MOVE_DIR Dir;
public:

	void ProcessMoveData(UCHAR _Data);
};

