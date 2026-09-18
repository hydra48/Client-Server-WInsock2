#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")


struct sockaddr_in addr;

int main()
{
	WSAData data = { 0 };
	if (WSAStartup(2.2, &data) == 0) {
		std::cout << "succes wsastratup" << std::endl;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777);
	if (bind(s, (sockaddr*)&addr, sizeof(addr)) == 0) {
		std::cout << "succes bind" << std::endl;

	}
	if (listen(s, 10) == 0) {
		std::cout << "succes listen" << std::endl;

	}
	int size = sizeof(addr);
	while (true) {
		try
		{
			SOCKET acpt = accept(s, (sockaddr*)&addr, &size);

		}
		catch (const std::exception&)
		{
			std::cout << "socket deja accepte" << std::endl;
		}
		Sleep(1000);
	}
	


}