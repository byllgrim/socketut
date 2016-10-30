#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

void die(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void handleconn(int fd) {
	char buf[BUFSIZ+1];
	char msg[] = "ho\n";

	bzero(buf, sizeof(buf));
	read(fd, buf, BUFSIZ);
	printf("%s", buf);

	send(fd, msg, sizeof(msg), 0);
}

int main(int argc, char *argv[]) {
	/* variables */
	struct addrinfo hints;
	struct addrinfo *res;
	int err;
	int sockfd, tmpfd;
	int one = 1;
	pid_t pid;

	/* take port as argument */
	if (argc != 2) {
		fprintf(stderr, "usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* construct address */
	hints.ai_family = AF_UNSPEC;     /* ipv4 or ipv6 */
	hints.ai_socktype = SOCK_STREAM; /* tcp */
	hints.ai_protocol = 0;           /* any protocol */
	hints.ai_flags = AI_PASSIVE;     /* any address */
	if ((err = getaddrinfo(NULL, argv[1], &hints, &res))) {
		fprintf(stderr, "getaddrinfo: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

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
	for (;;) {
		if ((tmpfd = accept(sockfd, 0, 0)) < 0) {
			perror("");
			continue;
		}

		switch (pid = fork()) {
		case -1:
			die("fork");
		case 0:
			close(sockfd);
			handleconn(tmpfd);
			close(tmpfd);
			exit(EXIT_SUCCESS);
		default:
			close(tmpfd);
		}
	}

	/* cleanup */
	freeaddrinfo(res);
	close(sockfd);
	/* TODO something forgotten? */
	/* TODO this is never reached */

	return 0;
}
