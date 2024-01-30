#pragma once

struct Vec2
{
	float x;
	float y;

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{

	}

	void operator+=(float _num)
	{
		this->x += _num;
		this->y += _num;
	}

	void operator-=(float _num)
	{
		this->x -= _num;
		this->y -= _num;
	}

	void operator*=(float _num)
	{
		this->x *= _num;
		this->y *= _num;
	}

	void operator/=(float _num)
	{
		this->x /= _num;
		this->y /= _num;
	}
};

enum KEY
{
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,

	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	TAB,
	LSHIFT,
	LALT,
	LCTRL,
	SPACE,

	ESC,
	ENTER,
	BACK,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	LBTN,
	RBTN,
	MBTN,

	KEY_END,
};

enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};