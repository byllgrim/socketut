#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[]) {
	int sockfd;
	struct addrinfo *res;
	char buf[256];

	if (getaddrinfo(NULL, argv[1], 0, &res))
		perror("getaddrinfo: error");

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) perror("failed to open socket");

	if (connect(sockfd, res->ai_addr, res->ai_addrlen))
		perror("connect: error");
	freeaddrinfo(res);

	if (send(sockfd, "hey\n", 4, 0) == -1) perror("failed to send");

	bzero(buf, 256);
	int n = read(sockfd, buf, 255);
	if (n < 0) perror("failed to read from socket");
	printf("%s\n", buf);

	return 0;
}
