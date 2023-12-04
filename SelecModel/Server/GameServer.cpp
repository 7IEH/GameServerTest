#include "Common.h"

// ����ŷ �������
// winsock �ʱ�ȭ -> socket ����� -> server address ���� ->  bind -> listen -> accept�� �������� -> clientsocket �����ͼ� �ش� socket�� ������ �߶�
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

	// IPv4 ü��, TCP ����
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		HandleError("Socket CreateError");
		return 0;
	}

	// ���� �ɼ� ����(�� ���ŷ)
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
	
	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &clientSize);

		if (clientSocket == INVALID_SOCKET)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			HandleError("Accept Error");
			return 0;
		}

		cout << inet_ntoa(clientAddr.sin_addr) << " : " << ntohs(clientAddr.sin_port) << "���� �����߽��ϴ�!\n";

		while (true)
		{
			char recvBuffer[1000];
			int recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				HandleError("Recv Error");
				return 0;
			}
			else if (recvLen == 0)
			{
				cout << "Connectd Error";
				return 0;
			}
			cout << "RecvLen : " << recvLen << "\nRecv Data : " << recvBuffer << '\n';

			while (true)
			{
				err = ::send(clientSocket, recvBuffer, recvLen, 0);
				if (err == SOCKET_ERROR)
				{
					if(::WSAGetLastError() == WSAEWOULDBLOCK)
						continue;

					HandleError("Send Error");
					return 0;
				}

				cout << "Send Data!";
				break;
			}
		}
	}
	
	WSACleanup();
	return 0;
}