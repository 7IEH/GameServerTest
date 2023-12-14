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

	vector<Session> s;
	s.resize(15);

	fd_set read;
	fd_set write;

	while (true)
	{
		FD_ZERO(&read);
		FD_ZERO(&write);

		FD_SET(listenSocket, &read);

		for (Session& ss : s)
		{
			if (ss.m_Sock == NULL)
				continue;


			FD_SET(ss.m_Sock, &read);
			if (ss.recvLen > 0)
			{
				FD_SET(ss.m_Sock, &write);
			}
		}

		// select socket ���� buffer�� �غ� �Ϸ�� ����
		// read ��, recvbuffer(Ŀ�ο���)���� userbuffer�� �����͸� �������� ���
		// write ��, sendbuffer�� ���� �� recvbuffer�� �����͸� ������ ���( ��, ���� �� recvbuffer�� �����Ͱ� �������� ���� ���)
		err = ::select(0, &read, &write, NULL, NULL);
		if (err == SOCKET_ERROR)
		{
			return 0;
		}

		if (FD_ISSET(listenSocket, &read) > 0)
		{
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &clientSize);
			cout << inet_ntoa(clientAddr.sin_addr) << " : " << ntohs(clientAddr.sin_port) << "���� �����߽��ϴ�!\n";
			s[_index].m_Sock = clientSocket;
			s[_index].index = _index;
			_index++;
		}

		for (Session& ss : s)
		{
			if (FD_ISSET(ss.m_Sock, &read) > 0)
			{
				int recvLen = recv(ss.m_Sock, ss.buffer, sizeof(ss.buffer), 0);
				cout << ss.buffer << "����\n";
				ss.recvLen = recvLen;
			}
		}

		// send�� ������ ��� recv buffer �����������
		for (Session& ss : s)
		{
			if (FD_ISSET(ss.m_Sock, &write) > 0)
			{
				if (!ss._register)
				{
					ss._register = true;
					char* buffer = (char*)&ss.index;
					int sendLen = send(ss.m_Sock, buffer, sizeof(ss.index), 0);
					cout << sendLen << '\n';
					continue;
				}

				for (Session& _ss : s)
				{
					if (&_ss == &ss)
					{
						continue;
					}
					else
					{
						int sendLen = send(_ss.m_Sock, &ss.buffer[ss.sendLen], ss.recvLen - ss.sendLen, 0);
						cout << ss.buffer[ss.sendLen] << "�۽�\n";
						ss.sendLen += sendLen;
					}
				}
				ss.recvLen = 0;
				ss.sendLen = 0;
			}
		}
	}

	WSACleanup();
	return 0;
}