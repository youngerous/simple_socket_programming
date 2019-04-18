#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <fcntl.h>

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib") //Winsock Library

#define BUFSIZE 1024
#define PORT 12346

/**	CLIENT */
int main(void){
	WSADATA wsaData;
	SOCKET socket_client;
	SOCKADDR_IN server_address;

	char message[30];
	int strLen;
	char file_name[] = "./file.txt";
	int received_file;
	char buffer[BUFSIZE];
	int total = 0;

	/* Initializing socket library */
	printf("Initializing...");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup() error...\n");
		exit(0);
	}

	printf("Initialized.\n");

	/* Create socket */
	printf("Creating socket...");
	socket_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (socket_client == INVALID_SOCKET) {
		printf("socket() error...\n");
		exit(0);
	}
	printf("Socket created.\n");

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	server_address.sin_port = htons(PORT);

	/* Request connect to server */
	printf("Connecting with server...");
	if (connect(socket_client, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		printf("connect() error...\n");
		exit(0);
	}
	printf("Connected.\n");

	received_file = open(file_name, O_CREAT | _O_EXCL | O_WRONLY);
	if (!received_file) {
		printf("Receive file error...\n");
		exit(0);
	}

	strLen = recv(socket_client, message, sizeof(message)-1, 0); // message from server
	if (strLen == -1) {
		printf("read() error...\n");
		exit(0);
	}

	printf("Content of the file: %s", message);
	write(received_file, message, sizeof(message)-1);
	
	/* Close socket library */
	closesocket(socket_client);
	WSACleanup();
	close(received_file);

	return 0;

}

