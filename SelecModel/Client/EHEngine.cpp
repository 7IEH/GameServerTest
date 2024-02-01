#include "pch.h"
#include "EHEngine.h"
#include "EHGameObject.h"
#include "EHInputMgr.h"
#include "EHTimeMgr.h"
#include "EHNetWorkMgr.h"

Engine::Engine()
	:m_Resolution(0.f, 0.f)
	, m_Hwnd(nullptr)
	, m_hDC(nullptr)
	, m_pDC(nullptr)
	, m_Bitmap(nullptr)
{
	m_Objects.resize(15);
}

Engine::~Engine()
{
}

HRESULT Engine::Init(HWND _Hwnd, Vec2 _resolution)
{
	m_Hwnd = _Hwnd;
	m_Resolution = _resolution;

	m_hDC = GetDC(m_Hwnd);

	m_Bitmap = CreateCompatibleBitmap(m_hDC, (int)m_Resolution.x, (int)m_Resolution.y);
	m_pDC = CreateCompatibleDC(m_hDC);
	HBITMAP _prevBitmap = (HBITMAP)SelectObject(m_pDC, m_Bitmap);
	DeleteObject(_prevBitmap);

	Initialize();

	return S_OK;
}

void Engine::Progress()
{
	Update();
	Render();
}

void Engine::MoveObjectX(int _register, int _dir)
{
	if (m_Objects.size() <= _register)
		return;

	GameObject* _obj = Engine::GetInst()->GetRegisterObject(_register);
	Vec2 _temp = _obj->GetDIr();
	_temp.x = _dir;
	_obj->SetDir(_temp);
}

void Engine::MoveObjectY(int _register, int _dir)
{
	if (m_Objects.size() <= _register)
		return;

	GameObject* _obj = Engine::GetInst()->GetRegisterObject(_register);
	Vec2 _temp = _obj->GetDIr();
	_temp.y = _dir;
	_obj->SetDir(_temp);
}

void Engine::Initialize()
{
	TimeMgr::GetInst()->Awake();
	InputMgr::GetInst()->Awake();
	NetWorkMgr::GetInst()->Awake();
}

void Engine::Update()
{
	TimeMgr::GetInst()->Update();
	InputMgr::GetInst()->Update();
	NetWorkMgr::GetInst()->Update();

	for (size_t i = 0;i < m_Objects.size();i++)
	{
		if(m_Objects[i]!=nullptr)
			m_Objects[i]->Update();
	}
}

void Engine::Render()
{
	Rectangle(m_pDC, -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);

	for (size_t i = 0;i < m_Objects.size();i++)
	{
		if (m_Objects[i] != nullptr)
			m_Objects[i]->Render(m_pDC);
	}

	BitBlt(m_hDC, 0, 0, (int)m_Resolution.x, (int)m_Resolution.y, m_pDC, 0, 0, SRCCOPY);
}


