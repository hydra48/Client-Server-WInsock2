#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 



struct sockaddr_in addr;
SOCKET s;
CRITICAL_SECTION lock;

DWORD WINAPI receive(LPVOID socket) {
	char buffer[4097];
	char sizebuffer[17];
	int receivedTotal = 0;
	int realsize = 0;
	std::vector<char> totalbuffer;
	while (true) {
		int sizereceived = 0;
		while (sizereceived < 16) {
			int bytereceive = recv(s, sizebuffer + sizereceived, 16 - sizereceived, 0);
			if (bytereceive <= 0) {
				return 0;
			}

			sizereceived = sizereceived + bytereceive;
		}
		sizebuffer[16] = '\0';
		std::string commands = sizebuffer;
		if (commands.rfind("siz", 0) == 0) {

			std::cout << sizebuffer << std::endl;

			realsize = std::stoi(commands.substr(3));
			totalbuffer.resize(realsize);
			receivedTotal = 0;

		}
		while (receivedTotal < realsize) {
			int remaining = realsize - receivedTotal;
			int toreceive = min(4096, remaining);
			int bytereceive = recv(s, buffer, toreceive, 0);
			if (bytereceive <= 0) {
				return 0;
			}

			memcpy(totalbuffer.data() + receivedTotal, buffer, bytereceive);
			receivedTotal = receivedTotal + bytereceive;

		}
		if (receivedTotal == realsize) {
			std::cout << "message : " << std::endl;
			std::cout.write(totalbuffer.data(), receivedTotal);
			std::cout << std::endl;
			receivedTotal = 0;
			realsize = 0;
			totalbuffer.clear();
		}
	}
	return 0;
}


bool isconnect(SOCKET sock)
{
	char buff[1];
	int ret = recv(sock, buff, sizeof(buff), MSG_PEEK);
    if (ret == 0)
	{
		std::cout << "socket ferme" << std::endl;
		return false;
	}
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}


int main()
{
	InitializeCriticalSection(&lock);

	WSAData data = { 0 };
	if (WSAStartup(2.2, &data) == 0) {
		std::cout << "succes wsastratup" << std::endl;
	}
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(7777);
	
	while (true) {
		if (isconnect(s) == false) {
			if (connect(s, (sockaddr*)&addr, sizeof(addr)) == 0) {
				std::cout << "socket connecte" << std::endl;
				HANDLE thread = CreateThread(NULL, 0, receive, &s, 0, NULL);

			}
			else {
				std::cout << "erreur connexion : " << WSAGetLastError() << std::endl;
				break;
			}
		}
	}
	DeleteCriticalSection(&lock);


}




