/**
 * A simple single-threaded TCP server that responses to client commands:
 * - ping
 * - time
 * - close
 * 
 * Kien @ 4/2023
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int close_server = 0;


//ham giao tiep voi client
void communication(int sock) {
	char buff[128];
	bzero(buff, sizeof(buff));
	read(sock, buff, sizeof(buff));

	printf("command: %s\n", buff);

	if (strcmp(buff, "ping") == 0) {
		strcpy(buff, "pong");

	} else if (strcmp(buff, "time") == 0) {
		time_t t = time(NULL);
		strcpy(buff, ctime(&t));
		buff[strlen(buff) - 1] = 0;	// remove \n from ctime()

	} else if (strcmp(buff, "close") == 0) {
		close_server = 1;
		strcpy(buff, "ok");

	} else {
		strcpy(buff, "invalid command");
	}

	write(sock, buff, sizeof(buff));
	printf("response: %s\n", buff);
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: tcp-server PORT\n");
		return 0;
	}

	//co 2 loai socket: socket cho, de client ket noi
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "socket creation failed\n");
		return 1;
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	//moi interface, kem theo 1 dia chi IP khac nhau. client co the ket noi voi bat ki 1 interface nao
	// khi muon client ket noi voi cu the 1 interface nao do thi se khac 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	//gan socket vao dia chi ta mong muon
	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		fprintf(stderr, "socket bind failed\n");
		return 1;
	}

	printf("server is running...\n");


	//chua co ket noi: se cho, co ket noi, se ket thuc
	while (!close_server) {
		if (listen(sockfd, 5) != 0) {
			fprintf(stderr, "Listen failed\n");
			return 1;
		}
		
		//socket moi: giao tiep voi client
		// server nay dang xu li tung client doc lap voi nhau
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int connfd = accept(sockfd, (struct sockaddr*)&client, &len);
		if (connfd < 0) {
			fprintf(stderr, "server accept failed\n");
			return 1;
		}

		printf("client connection accepted\n");
		communication(connfd);

		close(connfd);
	}

	//close 
	close(sockfd);
	return 0;
}