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

	for (int i = 0;i < s.size();i++)
	{
		s[i].m_Sock = NULL;
	}

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

			NetBuffer _register = {};
			_register._task = htonl(0);
			_register._dir[0] = htonf(0.f);
			_register._dir[0] = htonf(0.f);
			_register._pos[0] = htonf(0.f);
			_register._pos[0] = htonf(0.f);
			_register._register = htonl(_index);
			
			::send(clientSocket, (char*)&_register, sizeof(_register), 0);

			_register._task =  htonl(1);

			s[_index].m_Buffer = _register;
			s[_index].recvLen = sizeof(_register);
			_index++;
		}

		for (Session& ss : s)
		{
			if (FD_ISSET(ss.m_Sock, &read) > 0)
			{
				int recvLen = recv(ss.m_Sock, (char*)&ss.m_Buffer, sizeof(ss.m_Buffer), 0);

				NetBuffer _recvData = {};

				_recvData._task = ntohl(ss.m_Buffer._task);
				_recvData._dir[0] = ntohf(ss.m_Buffer._dir[0]);
				_recvData._dir[1] = ntohf(ss.m_Buffer._dir[1]);
				_recvData._pos[0] = ntohf(ss.m_Buffer._pos[0]);
				_recvData._pos[1] = ntohf(ss.m_Buffer._pos[1]);
				_recvData._register = ntohl(ss.m_Buffer._register);

				cout << _recvData._task << '\n';
				cout << _recvData._dir[0] << '\n';
				cout << _recvData._dir[1] << '\n';
				cout << _recvData._pos[0] << '\n';
				cout << _recvData._pos[1] << '\n';
				cout << _recvData._register << '\n';
				cout << "����" << '\n';

				ss.recvLen = recvLen;
			}
		}

		// send�� ������ ��� recv buffer �����������
		for (Session& ss : s)
		{
			if (ss.m_Sock == NULL)
				continue;

			if (FD_ISSET(ss.m_Sock, &write) > 0)
			{
				for (Session& _ss : s)
				{
					if (_ss.m_Sock == NULL)
						continue;

					if (&_ss == &ss)
					{
						continue;
					}
					else
					{
						NetBuffer recvData = {};

						int sendLen = send(_ss.m_Sock, (char*)&ss.m_Buffer, sizeof(ss.m_Buffer), 0);
						cout << "�۽�" << '\n';
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