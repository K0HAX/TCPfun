 /* 
 *  Compile with -pthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

#define PORT    "3000" /* Port to listen on */
#define BACKLOG     10  /* Passed to listen() */
#define MAX_LINE (256)

void *handle(void *pnewsock)
{
	int sock = *(int *) pnewsock;

	free(pnewsock);

	char buff[MAX_LINE];
	int n;
	/* send(), recv(), close() */
	while (1) {
		bzero(buff,256);
		n = -1;
		n = read(sock, buff, 255);
		if (n < 0)
		{
			perror("Error reading from socket");
			break;
		}

		if(strcmp(buff, ".exit") == 0)
		{
			n = write(sock, "I got the message",17);
			if (n < 0)
	                {
	                        perror("Error writing to socket");
	                        break;
	                }
			break;
		}

		printf("Message: %s\n",buff);

		n = write(sock, "I got the message",17);
		if (n < 0)
		{
			perror("Error writing to socket");
			break;
		}
		//Readline(sock, buff, MAX_LINE-1);
		//Writeline(sock, buff, strlen(buff));
		//printf("%s\r\n", buff);
	}
	printf("Released connection\r\n");
	return NULL;
}

int main(void)
{
    int sock;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */

    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 0;
    }

    freeaddrinfo(res);

    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 0;
    }

    /* Main loop */
    while (1) {
	pthread_t thread;
        size_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock_fd = accept(sock, (struct sockaddr*)&their_addr, &size);
        if (newsock_fd == -1) {
            perror("accept");
	    exit(-1);
        }
        else {
            printf("Got a connection from %s on port %d\n",
                    inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));

	    int *newsock = malloc(sizeof(int));
	    *newsock = newsock_fd;
            if (pthread_create(&thread, NULL, handle, newsock) != 0) {
                fprintf(stderr, "Failed to create thread\n");
            } else {
		printf("Detaching thread\n");
	    	pthread_detach(thread);
	    }
        }
    }

    close(sock);

    return 0;
}
