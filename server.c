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
	/* variables */
	struct addrinfo hints;
	struct addrinfo *res;
	int sockfd, tmpfd;
	int one = 1;
	char buf[BUFSIZ+1];

	/* take port as argument */
	errno = EINVAL;
	if (argc != 2)
		die("usage: client port");

	/* construct address */
	hints.ai_family = AF_UNSPEC;     /* ipv4 or ipv6 */
	hints.ai_socktype = SOCK_STREAM; /* tcp */
	hints.ai_protocol = 0;           /* any protocol */
	hints.ai_flags = AI_PASSIVE;     /* any address */
	if (getaddrinfo(NULL, argv[1], &hints, &res))
		die("getaddrinfo"); /* TODO doesnt set errno */

	/* create socket */
	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if (sockfd < 0)
		die("socket");

	/* set reuse */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
		die("setsockopt");

	/* bind to socket */
	if (bind(sockfd, res->ai_addr, res->ai_addrlen))
		die("bind");

	/* listen for connections */
	if (listen(sockfd, SOMAXCONN))
		die("listen");

	/* accept connections */
	bzero(buf, sizeof(buf));
	for (;;) {
		tmpfd = accept(sockfd, 0, 0);
		/* TODO handle error */

		read(tmpfd, buf, BUFSIZ);
		printf("%s\n", buf);
	}

	/* cleanup */
	freeaddrinfo(res);

	return 0;
}
