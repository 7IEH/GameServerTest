#pragma once

enum class State
{
	Idle,
	Walk,
};

class GameObject
{
private:
	Vec2	m_vPos;
	Vec2	m_vScale;
	Vec2	m_Dir;

	State	m_State;
	bool	m_Client;
	int		m_Register;
	
	int		m_Key[4];

public:
	void SetPosition(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPosition() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetClient(bool _flag) { m_Client = _flag; }

	Vec2 GetDIr() { return m_Dir; }
	void SetDir(Vec2 _dir) { m_Dir = _dir; }

	void SetRegister(int _register) { m_Register = _register; }
	int GetRegister(int _register) { return m_Register; }

private:
	void Idle();
	void Walk();

	void OtherIdle();
	void OtherWalk();

public:
	virtual void Update();
	virtual void Render(HDC _dc);

public:
	GameObject();
	~GameObject();
};

