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

	SOCKADDR_IN serverAddr = {};
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(7777);

	err = ::connect(clientSocekt, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	if (err == SOCKET_ERROR)
	{
		HandleError("Connect CreateError");
		return 0;
	}

	cout << "Success Connectd Server!" << '\n';

	while (true)
	{

	}

	return 0;
}