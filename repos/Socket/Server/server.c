#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <fcntl.h>


#define PORT 12346
#define BUFSIZE 1024

#pragma comment(lib, "ws2_32.lib") //Winsock Library
#pragma warning(disable:4996)


/**	SERVER */
int main(void){
	WSADATA wsaData;
	SOCKET socket_server, socket_client;
	SOCKADDR_IN address_server, address_client;

	int size_cli_addr;
	char file_name[] = "file.txt";
	char message[] = "Hello World!";
	int sending_file;
	char buffer[BUFSIZE];

	/* Initializing socket library */
	printf("Initializing...");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup() error...\n");
		exit(0);
	}
	printf("Initialized.\n");

	/* Create socket */
	printf("Creating socket...");
	socket_server = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_server == INVALID_SOCKET) {
		printf("socket() error...\n");
		exit(0);
	}
	printf("Socket created.\n");

	memset(&address_server, 0, sizeof(address_server)); 
	address_server.sin_family = AF_INET;
	address_server.sin_addr.s_addr = htonl(INADDR_ANY);
	address_server.sin_port = htons(PORT);

	/* Bind IP & PORT to socket */
	if (bind(socket_server, (SOCKADDR*)&address_server, sizeof(address_server)) == SOCKET_ERROR) {
		printf("bind() error...\n");
		exit(0);
	}

	/* Listen to client */
	if (listen(socket_server, 5) == SOCKET_ERROR) {
		printf("listen() error...\n");
		exit(0);
	}

	size_cli_addr = sizeof(address_client);
	printf("Waiting for client...\n");

	socket_client = accept(socket_server, (SOCKADDR*)&address_client, &size_cli_addr);

	if (socket_client == INVALID_SOCKET) {
		printf("accept() error...\n");
		exit(0);
	}
	
	/* Send file */
	sending_file = open(file_name, O_RDONLY);
	if (!sending_file) {
		printf("File open error...\n");
		exit(0);
	}
		
	memset(buffer, 0, BUFSIZE);
	read(sending_file, buffer, BUFSIZE); // fill the buffer
	printf("\nread: %s\n", buffer);
	send(socket_client, buffer, BUFSIZE, 0); // send buffer to client
	printf("Complete sending file.\n");

	/* Close socket library */
	closesocket(socket_client);
	closesocket(socket_server);
	WSACleanup();
	close(sending_file);

	return 0;
}



