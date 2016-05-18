#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) perror("failed to open socket");

	struct addrinfo *res = malloc(sizeof(res));
	if (getaddrinfo("127.0.0.1", "12321", 0, &res))
		perror("failed to get address");

	if (connect(sockfd, res->ai_addr, res->ai_addrlen))
		perror("failed to connect");
	freeaddrinfo(res);

	if (send(sockfd, "hey\n", 4, 0) == -1) perror("failed to send");

	char buf[256];
	bzero(buf, 256);
	int n = read(sockfd, buf, 255);
	if (n < 0) perror("failed to read from socket");
	printf("%s\n", buf);

	return 0;
}
