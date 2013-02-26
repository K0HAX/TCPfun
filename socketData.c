#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socketData.h"

struct _socket_data {
	int newsock;
	char ipAddr[256];
	unsigned int port;
};

socket_data *SocketDataNew(int sock, char ip[256], unsigned int p) {
	socket_data *sData = malloc(sizeof(socket_data));
	sData->newsock = sock;
	//sData->ipAddr = ip;
	strcpy(sData->ipAddr, ip);
	sData->port = p;

	return sData;
}

int getSocketDataSocket(socket_data *sData) {
	int nSock;
	nSock = sData->newsock;
	return nSock;
}

int getSocketDataIP(socket_data *sData, char buf[]) {
	strcpy(buf, sData->ipAddr);
	return 0;
}

int getSocketDataPort(socket_data *sData) {
	return sData->port;
}
