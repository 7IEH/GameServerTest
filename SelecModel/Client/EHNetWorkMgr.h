#pragma once

struct NetBuffer
{
	NetBuffer()
		:_task(1)
		,_dir(1.f,1.f)
	{

	};

	int _task;
	Vec2 _dir;
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

public:
	void AddNetTask(NetBuffer _netbuffer)
	{
		_netbuffer._task = htonl(_netbuffer._task);
		_netbuffer._dir.x = htonf(_netbuffer._dir.x);
		_netbuffer._dir.y = htonf(_netbuffer._dir.y);

		m_Send.push_back(_netbuffer);
	}

	NetBuffer AddRecvData(NetBuffer _netbuffer)
	{
		_netbuffer._task = ntohl(_netbuffer._task);
		_netbuffer._dir.x = ntohf(_netbuffer._dir.x);
		_netbuffer._dir.y = ntohf(_netbuffer._dir.y);

		return _netbuffer;
	}

public:
	void Awake();
	void Update();
	void Render();
};

