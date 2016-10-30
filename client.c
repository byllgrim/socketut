#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void die(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	struct addrinfo hints;
	struct addrinfo *res;
	int err;
	int sockfd;
	char buf[BUFSIZ+1];

	/* construct address */
	hints.ai_family = AF_UNSPEC;     /* ipv4 or ipv6 */
	hints.ai_socktype = SOCK_STREAM; /* tcp */
	hints.ai_protocol = 0;	   /* any protocol */
	hints.ai_flags = AI_PASSIVE;     /* any address */
	if ((err = getaddrinfo(NULL, argv[1], &hints, &res))) {
		fprintf(stderr, "getaddrinfo: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	/* create socket */
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
		die("socket");

	/* connect to address */
	if (connect(sockfd, res->ai_addr, res->ai_addrlen))
		die("connect");
	freeaddrinfo(res);

	/* send message */
	if (send(sockfd, "hey\n", 4, 0) < 0)
		die("send");

	/* read reply */
	bzero(buf, sizeof(buf));
	if (read(sockfd, buf, BUFSIZ) < 0)
		die("read");
	printf("%s", buf);

	return 0;
}
