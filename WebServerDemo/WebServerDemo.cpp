#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


int main()
{
	int serverSocket, clientSocket;
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
		WSACleanup();
	}

	serverSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		std::cout << "Error";

	std::string ip = "127.0.0.1";

	struct sockaddr_in serv_addr, client_addr;
	uint16_t port = 1256;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	serv_addr.sin_port = htons(port);

	::bind(serverSocket, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	::listen(serverSocket, 2);

	while (true)
	{
		socklen_t lengh = sizeof client_addr;
		clientSocket = ::accept(serverSocket, (SOCKADDR*)&client_addr, &lengh);

		char event[10] = { 0 };
		char messageToClient[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n<h1>Hello, client! Welcome to the Virtual Machine Web..</h1> \r\n\r\n";

		/*unsigned long on = 0;
		ioctlsocket(clientSocket, FIONBIO, &on);*/

		int on = 1;
		setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof on);

		::send(clientSocket, messageToClient, strlen(messageToClient), 0);
		while (recv(clientSocket, event, 10, 0) > 0) {
			std::cout << event;
		}
		std::cout << "End";
		closesocket(clientSocket);
	}
}
