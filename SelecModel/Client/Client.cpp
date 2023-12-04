#include "Common.h"

// 논블로킹 방식으로
// winsock 초기화 -> socket 만들기 -> server address 설정 -> connect 하기

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
	SOCKET clientSocekt = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocekt == INVALID_SOCKET)
	{
		HandleError("Socket CreateError");
		return 0;
	}

	// Nonblocking socket argp == 1(except 0)
	// blocking socket argp == 0

	ioctlsocket(clientSocekt, FIONBIO, &NONBLOCKING);

	SOCKADDR_IN serverAddr = {};
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(7777);

	while (true)
	{
		err = ::connect(clientSocekt, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
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
	
	cout << "Success Connectd Server!" << '\n';

	while (true)
	{
		char sendBuffer[1000] = "";
		cin >> sendBuffer;
		err = ::send(clientSocekt, sendBuffer, sizeof(sendBuffer), 0);
		if (err == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			HandleError("Send Error");
			return 0;
		}

		cout << "SendData\n";
		
		while (true)
		{
			char recvBuffer[1000];
			err = recv(clientSocekt, recvBuffer, sizeof(recvBuffer), 0);
			if (err == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				HandleError("Recv Error");
				return 0;
			}

			cout << "RecvData : " << recvBuffer << '\n';
			break;
		}
	}

	WSACleanup();
	return 0;
}