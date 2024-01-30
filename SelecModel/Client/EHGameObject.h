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

	State	m_State;
public:
	void SetPosition(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPosition() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

private:
	void Idle();
	void Walk();

public:
	virtual void Update();
	virtual void Render(HDC _dc);

public:
	GameObject();
	~GameObject();
};

