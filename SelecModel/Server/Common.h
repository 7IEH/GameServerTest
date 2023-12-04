#pragma once

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

void HandleError(string s)
{
	int err = ::WSAGetLastError();
	cout << s << " Error" << ", ErrorCode : " << err << '\n';
}

struct Session
{
	SOCKET m_Sock;
};

u_long BLOCKING = 0;
u_long NONBLOCKING = 1;
