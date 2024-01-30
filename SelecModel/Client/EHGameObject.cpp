#include "pch.h"
#include "EHGameObject.h"
#include "EHInputMgr.h"
#include "EHTimeMgr.h"
#include "EHNetWorkMgr.h"

GameObject::GameObject()
	:m_vPos(1.f,1.f)
	,m_vScale(1.f,1.f)
	,m_State(State::Idle)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	switch (m_State)
	{
	case State::Idle:
	{
		Idle();
	}
		break;
	case State::Walk:
	{
		Walk();
	}
		break;
	default:
		break;
	}
}

void GameObject::Render(HDC _dc)
{
	Vec2 LT = Vec2(m_vPos.x - m_vScale.x / 2.f, m_vPos.y - m_vScale.y / 2.f);
	Vec2 RB = Vec2(m_vPos.x + m_vScale.x / 2.f, m_vPos.y + m_vScale.y / 2.f);

	Rectangle(_dc, LT.x, LT.y, RB.x, RB.y);
}

void GameObject::Idle()
{
	if (KEY_TAP(KEY::A))
	{
		m_State = State::Walk;
		NetBuffer m_Buffer;
		m_vPos.x -= 200.f * DT;
		m_Buffer._task = 3;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_vPos.x += 200.f * DT;
	}

	if (KEY_PRESSED(KEY::W))
	{
		m_vPos.y -= 200.f * DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		m_vPos.y += 200.f * DT;
	}
}

void GameObject::Walk()
{
	if (KEY_RELEASED(KEY::A))
	{
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::D))
	{
		m_vPos.x += 200.f * DT;
	}

	if (KEY_RELEASED(KEY::W))
	{
		m_vPos.y -= 200.f * DT;
	}

	if (KEY_RELEASED(KEY::S))
	{
		m_vPos.y += 200.f * DT;
	}
}