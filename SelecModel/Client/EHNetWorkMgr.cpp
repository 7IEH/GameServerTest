#include "pch.h"
#include "EHNetWorkMgr.h"
#include "EHGameObject.h"
#include "EHEngine.h"

NetWorkMgr::NetWorkMgr()
{

}

NetWorkMgr::~NetWorkMgr()
{
	WSACleanup();
}

void NetWorkMgr::Awake()
{
	// Winsock 초기화
	WSADATA wsData;
	HRESULT err = ::WSAStartup(MAKEWORD(2, 2), &wsData);
	assert(err == 0);

	// Connect 만들고
	m_Socket = ::socket(AF_INET, SOCK_STREAM, 0);
	assert(m_Socket != INVALID_SOCKET);

	u_long NONBLOCKING = 1;
	ioctlsocket(m_Socket, FIONBIO, &NONBLOCKING);

	SOCKADDR_IN _serverAddr = {};
	::memset(&_serverAddr, 0, sizeof(_serverAddr));
	_serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &_serverAddr.sin_addr);
	_serverAddr.sin_port = htons(7777);

	while (true)
	{
		int err = ::connect(m_Socket, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr));
		if (err == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			if (::WSAGetLastError() == WSAEISCONN)
				break;

			return;
		}
	}
}

void NetWorkMgr::Update()
{
	FD_ZERO(&m_ReadSet);
	FD_ZERO(&m_WriteSet);

	FD_SET(m_Socket, &m_ReadSet);
	FD_SET(m_Socket, &m_WriteSet);

	select(0, &m_ReadSet, &m_WriteSet, nullptr, 0);

	if (FD_ISSET(m_Socket, &m_ReadSet))
	{
		NetBuffer _recvBuffer = {};
		int recvLen = ::recv(m_Socket, (char*)&_recvBuffer, sizeof(_recvBuffer), 0);

		_recvBuffer = AddRecvData(_recvBuffer);

		cout << "수신" << '\n';
		cout << _recvBuffer._task << '\n';
		cout << _recvBuffer._dir.x << '\n';
		cout << _recvBuffer._dir.y << '\n';
	}
	else if (FD_ISSET(m_Socket, &m_WriteSet))
	{
		for (size_t i = 0;i < m_Send.size();i++)
		{
			NetBuffer _net = {};
			_net._task = ntohl(m_Send[i]._task);
			_net._dir.x = ntohf(m_Send[i]._dir.x);
			_net._dir.y = ntohf(m_Send[i]._dir.y);
			_net._pos.x = ntohf(m_Send[i]._pos.x);
			_net._pos.y = ntohf(m_Send[i]._pos.y);

			int err = ::send(m_Socket, (char*)&m_Send[i], sizeof(m_Send[i]), 0);

			if (err == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				return;
			}

			cout << "SendData\n";
		}
	}
	m_Send.clear();

	Update_Task();
}

void NetWorkMgr::Render()
{
}

void NetWorkMgr::Update_Task()
{
	for (size_t i = 0;i < m_Recv.size();i++)
	{
		int _task = m_Recv[i]._task;
		switch (_task)
		{
		case 0:
		{
			Update_Enter(m_Recv[i]);
		}
		break;
		case 1:
		{
			Update_AddObject(m_Recv[i]);
		}
			break;
		case 2:
		{
			Update_MoveX(m_Recv[i]);
		}
			break;
		case 3:
		{
			Update_MoveY(m_Recv[i]);
		}
			break;
		}
	}

	m_Recv.clear();
}

void NetWorkMgr::Update_Enter(NetBuffer _buffer)
{
	Vec2 _pos = Vec2(_buffer._pos.x,_buffer._pos.y);
	m_Register = _buffer._register;
	GameObject* _obj = new GameObject();
	_obj->SetPosition(_pos);
	_obj->SetScale(Vec2(20.f, 20.f));
	_obj->SetClient(TRUE);
	_obj->SetRegister(m_Register);
	Engine::GetInst()->AddObject(_obj, m_Register);
}

void NetWorkMgr::Update_AddObject(NetBuffer _buffer)
{
	if (nullptr != Engine::GetInst()->GetRegisterObject(_buffer._register))
		return;

	Vec2 _pos = Vec2(_buffer._pos.x, _buffer._pos.y);
	int _register = _buffer._register;
	GameObject* _obj = new GameObject();
	_obj->SetPosition(_pos);
	_obj->SetScale(Vec2(20.f, 20.f));
	_obj->SetClient(FALSE);
	_obj->SetRegister(_register);
	Engine::GetInst()->AddObject(_obj, _register);

	GameObject* _owner = Engine::GetInst()->GetRegisterObject(m_Register);
	NetBuffer _ownerBuffer = {};
	_ownerBuffer._pos = _owner->GetPosition();
	_ownerBuffer._register = m_Register;
	_ownerBuffer._task = 1;

	AddNetTask(_ownerBuffer);
}

void NetWorkMgr::Update_MoveX(NetBuffer _buffer)
{
	int _register = _buffer._register;
	Vec2 _dir = Vec2(_buffer._dir.x, _buffer._dir.y);
	Engine::GetInst()->MoveObjectX(_register, _dir.x);
}

void NetWorkMgr::Update_MoveY(NetBuffer _buffer)
{
	int _register = _buffer._register;
	Vec2 _dir = Vec2(_buffer._dir.x, _buffer._dir.y);
	Engine::GetInst()->MoveObjectY(_register, _dir.y);
}