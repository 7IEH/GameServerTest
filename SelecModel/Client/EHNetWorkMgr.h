#pragma once

struct NetBuffer
{
	NetBuffer()
		:_task(1)
		,_dir(0.f,0.f)
		,_pos(0.f,0.f)
	{

	};

	int _task;
	int _register;
	Vec2 _dir;
	Vec2 _pos;
};

class NetWorkMgr
	:public Singleton<NetWorkMgr>
{
	Single(NetWorkMgr)

private:
	SOCKET	m_Socket;

	fd_set	m_ReadSet;
	fd_set  m_WriteSet;
	
	vector<NetBuffer> m_Send;
	vector<NetBuffer> m_Recv;

	int				  m_Register;
public:
	vector<NetBuffer> GetRecvData() { return m_Recv; }
	
public:
	void AddNetTask(NetBuffer _netbuffer)
	{
		_netbuffer._task = htonl(_netbuffer._task);
		_netbuffer._dir.x = htonf(_netbuffer._dir.x);
		_netbuffer._dir.y = htonf(_netbuffer._dir.y);
		_netbuffer._pos.x = _netbuffer._pos.x;
		_netbuffer._pos.y = _netbuffer._pos.y;
		_netbuffer._register = htonl(_netbuffer._register);

		m_Send.push_back(_netbuffer);
	}

	NetBuffer AddRecvData(NetBuffer _netbuffer)
	{
		_netbuffer._task = ntohl(_netbuffer._task);
		_netbuffer._dir.x = ntohf(_netbuffer._dir.x);
		_netbuffer._dir.y = ntohf(_netbuffer._dir.y);
		_netbuffer._pos.x = _netbuffer._pos.x;
		_netbuffer._pos.y = _netbuffer._pos.y;
		_netbuffer._register = ntohl(_netbuffer._register);

		m_Recv.push_back(_netbuffer);

		return _netbuffer;
	}

public:
	void Awake();
	void Update();
	void Render();

private:
	void Update_Task();
	void Update_Enter(NetBuffer _buffer);
	void Update_MoveX(NetBuffer _buffer);
	void Update_MoveY(NetBuffer _buffer);
	void Update_AddObject(NetBuffer _buffer);
};

