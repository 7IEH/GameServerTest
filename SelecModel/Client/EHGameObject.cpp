#include "pch.h"
#include "EHGameObject.h"
#include "EHInputMgr.h"
#include "EHTimeMgr.h"
#include "EHNetWorkMgr.h"

GameObject::GameObject()
	: m_vPos(1.f, 1.f)
	, m_vScale(1.f, 1.f)
	, m_State(State::Idle)
	, m_Client(FALSE)
	, m_Register(0)
	, m_Dir(0.f, 0.f)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (m_Client)
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
	else
	{
		switch (m_State)
		{
		case State::Idle:
		{
			OtherIdle();
		}
		break;
		case State::Walk:
		{
			OtherWalk();
		}
		break;
		default:
			break;
		}
	}
}

void GameObject::Render(HDC _dc)
{
	Vec2 LT = Vec2(m_vPos.x - m_vScale.x / 2.f, m_vPos.y - m_vScale.y / 2.f);
	Vec2 RB = Vec2(m_vPos.x + m_vScale.x / 2.f, m_vPos.y + m_vScale.y / 2.f);
	HBRUSH _brush = nullptr;
	HBRUSH _oldBrush = nullptr;
	if (m_Client)
	{
		_brush = CreateSolidBrush(RGB(255.f, 255.f, 0.f));
		_oldBrush = (HBRUSH)SelectObject(_dc, _brush);
	}

	Rectangle(_dc, LT.x, LT.y, RB.x, RB.y);

	if (m_Client)
	{
		SelectObject(_dc, _oldBrush);
		DeleteObject(_brush);
	}
}

void GameObject::Idle()
{
	if (KEY_TAP(KEY::A))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = -1.f;
		m_Buffer._register = m_Register;
		m_Dir.x = -1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::D))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 1.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::W))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = -1.f;
		m_Buffer._register = m_Register;
		m_Dir.y = -1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::S))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 1.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_RELEASED(KEY::A))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::D))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::W))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::S))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::A) && m_Dir.x == -1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::D) && m_Dir.x == 1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::W) && m_Dir.y == -1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::S) && m_Dir.y == 1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}
}

void GameObject::Walk()
{
	if (KEY_TAP(KEY::A))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = -1.f;
		m_Buffer._register = m_Register;
		m_Dir.x = -1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::D))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 1.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::W))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = -1.f;
		m_Buffer._register = m_Register;
		m_Dir.y = -1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_TAP(KEY::S))
	{
		m_State = State::Walk;

		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 1.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 1.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
	}

	if (KEY_PRESSED(KEY::A))
	{
		m_vPos.x -= 200.f * DT;
		m_Dir.x = -1.f;
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_vPos.x += 200.f * DT;
		m_Dir.x = 1.f;
	}

	if (KEY_PRESSED(KEY::W))
	{
		m_vPos.y -= 200.f * DT;
		m_Dir.y = -1.f;
	}

	if (KEY_PRESSED(KEY::S))
	{
		m_vPos.y += 200.f * DT;
		m_Dir.y = 1.f;
	}

	if (KEY_RELEASED(KEY::A))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::D))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::W))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_RELEASED(KEY::S))
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::A) && m_Dir.x == -1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::D) && m_Dir.x == 1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 2;
		m_Buffer._dir.x = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.x = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::W) && m_Dir.y == -1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}

	if (KEY_NOTPRESSED(KEY::S) && m_Dir.y == 1)
	{
		NetBuffer m_Buffer;
		m_Buffer._task = 3;
		m_Buffer._dir.y = 0.f;
		m_Buffer._register = m_Register;
		m_Dir.y = 0.f;
		NetWorkMgr::GetInst()->AddNetTask(m_Buffer);
		m_State = State::Idle;
	}
}

void GameObject::OtherIdle()
{
	m_vPos.x += m_Dir.x * 200.f * DT;
	m_vPos.y += m_Dir.y * 200.f * DT;
}

void GameObject::OtherWalk()
{
}