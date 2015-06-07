#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "connection.h"
#include "util/assert_do.h"

struct conn {
	int sock;
};

static conn_t *new_conn(int sock) {
	conn_t *c = malloc(sizeof * c);
	c->sock = sock;
	return c;
}

conn_t *conn_listen(const char *ip, int port) {
	struct sockaddr_in addr;
	int sock;

	// Open a socket
	sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sock == -1) {
		perror("Could not open socket");
		return NULL;
	}

	// Bind the socket to the address
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	assert_do(inet_aton(ip, &addr.sin_addr));

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("Could not bind socket");
		return NULL;
	}

	// Listen on the socket!
	if (listen(sock, 10) == -1) {
		perror("Could not listen on socket");
		return NULL;
	}

	// Create the connection object
	return new_conn(sock);
};

conn_t *conn_connect(const char *ip, int port) {
	struct sockaddr_in addr;
	int sock;

	// Open a socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Could not open socket");
		return NULL;
	}

	// Bind the socket to the address
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	assert_do(inet_aton(ip, &addr.sin_addr));

	if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("Could not connect to server");
		return NULL;
	}

	// Create the connection object
	return new_conn(sock);
}

conn_t *conn_accept(conn_t *from) {
	int ret;
	struct sockaddr_in addr;
	socklen_t socklen;

	socklen = sizeof(struct sockaddr_in);
	ret = accept(from->sock, (struct sockaddr *) &addr, &socklen);

	if (ret != -1) {
		return new_conn(ret);
	}

	if (errno != EAGAIN && errno != EWOULDBLOCK) {
		perror("Error while accepting connection");
	}

	return NULL;
}

void conn_close(conn_t *c) {
	close(c->sock);
	free(c);
}

size_t conn_send(conn_t *c, const void *packet, size_t size) {
	ssize_t ret;
	size_t orig = size;

	while (size > 0) {
		ret = send(c->sock, packet, size, 0);
		if (ret < 0) {
			// Error occured
			// TODO Log an error or some jazz like that?
			break;
		}

		packet += ret;
		size -= ret;
	}

	return orig - size;

}

size_t conn_recv(conn_t *c, void *packet, size_t size) {
	ssize_t ret;
	size_t orig = size;

	while (size > 0) {
		ret = recv(c->sock, packet, size, 0);
		if (ret < 0) {
			// TODO log an error?
			break;
		}

		packet += ret;
		size -= ret;
	}

	return orig - size;
}
