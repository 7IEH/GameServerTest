#pragma once

class TimeMgr
	:public Singleton<TimeMgr>
{
	Single(TimeMgr)
private:
	float		m_fDeltaTime;

	LARGE_INTEGER		m_prevCounter;
	LARGE_INTEGER		m_curCounter;
	LARGE_INTEGER		m_Frequency;

	int				m_iCall;
	float			m_fTime;

public:
	float				GetDeltaTime() { return m_fDeltaTime; }

public:
	void				Awake();
	void				Update();
};