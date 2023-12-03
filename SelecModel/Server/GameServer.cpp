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

	while (true)
	{
		SOCKADDR_IN clientAddr = {};
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int clientSize = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &clientSize);

		if (clientSocket == INVALID_SOCKET)
		{
			HandleError("Accept Error");
			return 0;
		}

		cout << inet_ntoa(clientAddr.sin_addr) << " : " << ntohs(clientAddr.sin_port) << "���� �����߽��ϴ�!\n";
	}

	return 0;
}