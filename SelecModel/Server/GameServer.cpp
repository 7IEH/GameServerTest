#include "Common.h"

// 논블로킹 방식으로
// winsock 초기화 -> socket 만들기 -> server address 설정 ->  bind -> listen -> accept로 가져오고 -> clientsocket 가져와서 해당 socket을 가지고 야랄
//

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
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		HandleError("Socket CreateError");
		return 0;
	}

	// 소켓 옵션 설정(논 블록킹)
	ioctlsocket(listenSocket, FIONBIO, &NONBLOCKING);

	SOCKADDR_IN serverAddr = {};
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(7777);

	err = ::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	if (err == SOCKET_ERROR)
	{
		HandleError("Bind Error");
		return 0;
	}

	err = ::listen(listenSocket, 20);

	if (err == SOCKET_ERROR)
	{
		HandleError("Listen Error");
		return 0;
	}

	cout << "Client Connected Setting Success!" << '\n';

	SOCKADDR_IN clientAddr = {};
	::memset(&clientAddr, 0, sizeof(clientAddr));
	int clientSize = sizeof(clientAddr);
	
	vector<Session> s;

	fd_set read;
	fd_set write;

	while (true)
	{
		FD_ZERO(&read);
		FD_ZERO(&write);

		FD_SET(listenSocket, &read);

		for (Session ss : s)
		{
			FD_SET(ss.m_Sock, &read);
			FD_SET(ss.m_Sock, &write);
		}

		err = ::select(0, &read, &write, NULL, NULL);
		if (err == SOCKET_ERROR)
		{
			return 0;
		}

		if (FD_ISSET(listenSocket, &read) > 0)
		{
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &clientSize);
			cout << inet_ntoa(clientAddr.sin_addr) << " : " << ntohs(clientAddr.sin_port) << "님이 접속했습니다!\n";
			s.push_back(Session{clientSocket});
		}

		for (Session ss : s)
		{
			char recvBuffer[1000] = "";
			if (FD_ISSET(ss.m_Sock, &read) > 0)
			{
				recv(ss.m_Sock, recvBuffer, sizeof(recvBuffer), 0);
				cout << recvBuffer << "Test\n";
			}
		}


		// send의 조건이 상대 recv buffer 비어져있을때
		for (Session ss : s)
		{
			char sendBuffer[1000] = "Test";
			if (FD_ISSET(ss.m_Sock, &write) > 0)
			{
				for (Session ss : s)
				{
					send(ss.m_Sock, sendBuffer, sizeof(sendBuffer), 0);
					cout << sendBuffer << "Test\n";
				}
			}
		}
	}
	
	WSACleanup();
	return 0;
}