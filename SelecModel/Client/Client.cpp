#include "Common.h"

// ����ŷ �������
// winsock �ʱ�ȭ -> socket ����� -> server address ���� -> connect �ϱ�

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
			// ����ŷ�̱� ������ ��ȯ���� ���� �ʾƵ�
			// ���� ������ ó��
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// �̹� ������ ����� ������ �ٽ��ѹ� �ǵ� ���
			if (::WSAGetLastError() == WSAEISCONN)
				break;

			// ��¥ ������ ���
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