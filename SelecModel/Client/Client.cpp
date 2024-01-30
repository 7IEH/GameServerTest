#include "pch.h"
//#include "Common.h"
//
//// ����ŷ �������
//// winsock �ʱ�ȭ -> socket ����� -> server address ���� -> connect �ϱ�
//
//SOCKET clientSocket;
//
//void recvFunc();
//
//struct buffer
//{
//	int x;
//	int y;
//};
//
//int main()
//{
//	WSADATA wsData;
//	int err = ::WSAStartup(MAKEWORD(2, 2), &wsData);
//	if (err != 0)
//	{
//		HandleError("Winsock Initialize");
//		return 0;
//	}
//
//	// IPv4 ü��, TCP ����
//	clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (clientSocket == INVALID_SOCKET)
//	{
//		HandleError("Socket CreateError");
//		return 0;
//	}
//
//	// Nonblocking socket argp == 1(except 0)
//	// blocking socket argp == 0
//	ioctlsocket(clientSocket, FIONBIO, &NONBLOCKING);
//
//	SOCKADDR_IN serverAddr = {};
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
//	serverAddr.sin_port = htons(7777);
//
//
//	while (true)
//	{
//		err = ::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
//		if (err == SOCKET_ERROR)
//		{
//			// ����ŷ�̱� ������ ��ȯ���� ���� �ʾƵ�
//			// ���� ������ ó��
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//				continue;
//
//			// �̹� ������ ����� ������ �ٽ��ѹ� �ǵ� ���
//			if (::WSAGetLastError() == WSAEISCONN)
//				break;
//
//			// ��¥ ������ ���
//			HandleError("Connect CreateError");
//			return 0;
//		}
//	}
//
//	thread t1(recvFunc);
//
//	cout << "Success Connectd Server!" << '\n';
//
//	// recv�� ��� �ƴϸ� send���� cin���� �޾ƾ� �����
//	fd_set read;
//	fd_set write;
//
//	int dir = 0;
//
//	while (true)
//	{
//		dir = -1;
//		FD_ZERO(&read);
//		FD_ZERO(&write);
//
//		FD_SET(clientSocket, &read);
//		FD_SET(clientSocket, &write);
//
//		select(0, &read, &write, nullptr, 0);
//
//		if (FD_ISSET(clientSocket, &read))
//		{
//			char recvBuffer[1000] = {};
//			int err = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//			if (err == SOCKET_ERROR)
//			{
//				if (::WSAGetLastError() == WSAEWOULDBLOCK)
//					continue;
//
//				HandleError("Recv Error");
//				return 0;
//			}
//
//			cout << "RecvData : " << recvBuffer << '\n';
//		}
//		else if (FD_ISSET(clientSocket, &write))
//		{
//			buffer test;
//			test.x = 1;
//			test.y = 2;
//
//			test.x = htonl(test.x);
//			test.y = htonl(test.y);
//
//			err = ::send(clientSocket, (char*)&test, sizeof(buffer), 0);
//			if (err == SOCKET_ERROR)
//			{
//				if (::WSAGetLastError() == WSAEWOULDBLOCK)
//					continue;
//
//				HandleError("Send Error");
//				return 0;
//			}
//
//			cout << "SendData\n";
//		}
//	}
//
//	t1.join();
//
//	WSACleanup();
//	return 0;
//}
//
//void recvFunc()
//{
//	while (true)
//	{
//		
//	}
//}