#include <sys/types.h>
#include <sys/socket.h>
#include "serve.h"

struct server {
	int socket; // file descriptor

	// TODO need fields for connect/disconnect handlers
};

bool start_server(void)
{
	int socket;
	server_t * server;
	struct sockaddr_in addr;

	// Open a socket
	socket = socket(AF_INET, SOCK_STREAM, 0);
	if (socket == -1) {
		perror("Could not open socket");
		return NULL;
	}

	// Bind the socket to the address
	addr.sin_family = AF_INET;
	addr.sin_port = 1337;
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket, (struct sockaddr *) &addr, sizeof(sockaddr_in)) == -1) {
		perror("Could not bind socket");
		return NULL;
	}

	// Listen on the socket!
	if (listen(socket, 10) == -1) {
		perror("Could not listen on socket");
		return NULL;
	}

	server = malloc(sizeof(server_t));
	server->socket = socket;

	return server;
}

bool stop_server(server_t * server)
{
	// Close the connection
	if (close(server->socket) == -1) {
		perror("Could not close socket");
		return false;
	}

	return true;
}

// Called by tick_server upon each new connection found
static void new_connection(const struct sockaddr_in addr, soclen_t len)
{
	// TODO
}

void tick_server(server_t * server)
{
	int ret, accepted;
	struct sockaddr_in addr;
	socklen_t socklen;

	// Accept all pending connections
	do {
		socklen = sizeof(struct sockaddr_in);
		ret = accept(server->socket, (struct sockaddr *) &addr, &socklen, SOCK_NONBLOCK);

		accepted = (ret == EAGAIN || ret == EWOULDBLOCK);

		if (accepted) {
			new_connection(&addr, socklen);
		}
	} while (accepted);
}

void on_connect(server_t * server, bool (* handler)(int identifier))
{
	// TODO
}

void on_disconnect(server_t * server, void (* handler)(int identifier))
{
	// TODO
}
