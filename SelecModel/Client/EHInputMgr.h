#pragma once

#include <vector>
using std::vector;

class InputMgr
	:public Singleton<InputMgr>
{
	Single(InputMgr)
private:
	vector<FKeyData>	m_vecKeyData;

public:
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeyData[_Key].eState; }

public:
	void Awake();
	void Update();
};

