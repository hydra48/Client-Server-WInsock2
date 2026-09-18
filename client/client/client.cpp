#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 



struct sockaddr_in addr;

bool isconnect(SOCKET sock) {
	char buff[1];
	int ret = recv(sock, buff, sizeof(buff), 0);
	if(ret == 0 || ret == SOCKET_ERROR){
		std::cout << "socket ferme" << std::endl;
		return false;
	}
	else {
		return true;
	}
}


int main()
{
	WSAData data = { 0 };
	if (WSAStartup(2.2, &data) == 0) {
		std::cout << "succes wsastratup" << std::endl;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(7777);
	
	while (true) {
		if (isconnect(s) == false) {
			if (connect(s, (sockaddr*)&addr, sizeof(addr)) == 0) {
				std::cout << "socket connecte" << std::endl;
			}
			else {
				std::cout << "erreur connexion : " << WSAGetLastError() << std::endl;
			}
		}
	}

}


