#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "serve.h"
#include "proto.h"

int mmcmp(void *x, void *y, size_t s)
{
	size_t i;
	signed char *a = x;
	signed char *b = y;
	for (i = 0; i < s; i++) {
		printf("! %d %c %d %c\n", a[i], a[i], b[i], b[i]);
		if (a[i] != b[i]) {
			return a[i] - b[i];
		}
	}
	return 0;
}

int start_client(const char * ip, int port)
{
	int sock;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Could not open socket");
		exit(EXIT_FAILURE);
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = port;
	assert(inet_aton(ip, &addr.sin_addr));

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("Could not connect to server");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void handshake(int sock)
{
	iiag_handshake_t got;
	iiag_handshake_t sent = {
		IIAG_MARKER,
		IIAG_PROTO_MAJOR,
		IIAG_PROTO_MINOR
	};

	send(sock, &sent, sizeof(sent), 0);
	assert(recv(sock, &got, sizeof(got), 0) == sizeof(got));

	if (memcmp(&sent, &got, sizeof(iiag_handshake_t))) {
		fprintf(stderr, "Handshake mismatch.\n");
		exit(EXIT_FAILURE);
	}
}

int main()
{
	int sock, pid, status;
	server_t * server;

	pid = fork();

	if (pid) {
		// parent, server
		server = start_server();

		do {
			tick_server(server);
		} while(waitpid(pid, &status, WNOHANG) != pid);

		stop_server(server);

		// Check for a failure of the client
		if (!WIFEXITED(status) || WEXITSTATUS(status)) {
			fprintf(stderr, "Client exit status = %d\n", WEXITSTATUS(status));
			return EXIT_FAILURE;
		}
	} else {
		// child, client
		sock = start_client("127.0.0.1", 1337);
		handshake(sock);
	}

	return 0;
}
