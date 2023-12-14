#include "Common.h"

// 논블로킹 방식으로
// winsock 초기화 -> socket 만들기 -> server address 설정 -> connect 하기

SOCKET clientSocket;

void recvFunc();

struct buffer
{
	int x;
	int y;
};

int main()
{
	WSADATA wsData;
	int err = ::WSAStartup(MAKEWORD(2, 2), &wsData);
	if (err != 0)
	{
		HandleError("Winsock Initialize");
		return 0;
	}

	// IPv4 체계, TCP 연결
	clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket CreateError");
		return 0;
	}

	// Nonblocking socket argp == 1(except 0)
	// blocking socket argp == 0
	ioctlsocket(clientSocket, FIONBIO, &NONBLOCKING);

	SOCKADDR_IN serverAddr = {};
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(7777);


	while (true)
	{
		err = ::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
		if (err == SOCKET_ERROR)
		{
			// 논블로킹이기 때문에 반환값이 오지 않아도
			// 소켓 에러로 처리
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 이미 서버와 연결된 소켓을 다시한번 건든 경우
			if (::WSAGetLastError() == WSAEISCONN)
				break;

			// 진짜 오류일 경우
			HandleError("Connect CreateError");
			return 0;
		}
	}

	thread t1(recvFunc);

	cout << "Success Connectd Server!" << '\n';

	// recv는 계속 아니면 send에서 cin으로 받아야 진행됨
	fd_set read;
	fd_set write;

	int dir = 0;

	while (true)
	{
		dir = -1;
		FD_ZERO(&read);
		FD_ZERO(&write);

		FD_SET(clientSocket, &read);

		if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		{
			FD_ISSET(clientSocket, &write);
			dir = 0;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8001)
		{
			FD_ISSET(clientSocket, &write);
			dir = 1;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
		{
			FD_ISSET(clientSocket, &write);
			dir = 2;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8001)
		{
			FD_ISSET(clientSocket, &write);
			dir = 3;
		}

		select(0, &read, &write, nullptr, 0);

		if (FD_ISSET(clientSocket, &read))
		{
			char recvBuffer[1000] = {};
			int err = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (err == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				HandleError("Recv Error");
				return 0;
			}

			cout << "RecvData : " << recvBuffer << '\n';
		}
		else if (FD_ISSET(clientSocket, &write))
		{
			char lowbyte = (dir & 255);
			char highbyte = ((dir >> 8) & 255);
			char sendBuffer = highbyte;
			err = ::send(clientSocket, &sendBuffer, sizeof(sendBuffer), 0);
			if (err == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				HandleError("Send Error");
				return 0;
			}

			cout << "SendData\n";
		}
	}

	t1.join();

	WSACleanup();
	return 0;
}

void recvFunc()
{
	while (true)
	{
		
	}
}