#include "pch.h"
#include "EHNetWorkMgr.h"

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
}

void NetWorkMgr::Render()
{
}
