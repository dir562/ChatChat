#pragma once

class CObj;

class CEventMgr
{
	SINGLE(CEventMgr);
private:
	vector<tEvent>		m_vecEvent;
	vector<CObj*>		m_vecDeadObj;

public:
	void update();
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }

private:
	void ExcuteEvent(const tEvent& _event);
};

