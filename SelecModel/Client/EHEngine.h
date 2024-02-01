#pragma once


class GameObject;
class Engine
	:public Singleton<Engine>
{
	Single(Engine)
private:
	HWND	m_Hwnd;
	HDC		m_hDC;
	HDC		m_pDC;
	Vec2	m_Resolution;
	
	HBITMAP	m_Bitmap;

	vector<GameObject*>	m_Objects;
	
public:
	HRESULT Init(HWND _Hwnd, Vec2 _resolution);
	void Progress();

	void AddObject(GameObject* _obj,int _register) { m_Objects[_register] = _obj; }
	void MoveObjectX(int _register, int _dir);
	void MoveObjectY(int _register, int _dir);

	GameObject* GetRegisterObject(int _register) { return m_Objects[_register]; }
private:
	void Initialize();
	void Update();
	void Render();
};

