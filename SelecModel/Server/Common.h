#pragma once

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

struct NetBuffer
{
	int _task;
	float _dir[2];
};

void HandleError(string s)
{
	int err = ::WSAGetLastError();
	cout << s << " Error" << ", ErrorCode : " << err << '\n';
}

struct Session
{
	SOCKET m_Sock;
	NetBuffer m_Buffer;
	int recvLen = 0;
	int sendLen = 0;
	bool _register = false;
	int index;
};

static int _index = 0;

u_long BLOCKING = 0;
u_long NONBLOCKING = 1;
