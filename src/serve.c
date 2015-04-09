#include <assert.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "serve.h"
#include "proto.h"
#include "util/list.h"

struct server {
	int sock;
	list_t * connections;
	list_t * connect_handler;
	list_t * disconnect_handler;
};

bool start_server(void)
{
	int sock;
	server_t * server;
	struct sockaddr_in addr;

	// Open a socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Could not open socket");
		return NULL;
	}

	// Bind the socket to the address
	addr.sin_family = AF_INET;
	addr.sin_port = 1337;
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("Could not bind socket");
		return NULL;
	}

	// Listen on the socket!
	if (listen(sock, 10) == -1) {
		perror("Could not listen on socket");
		return NULL;
	}

	server = malloc(sizeof(server_t));
	server->sock = sock;

	return server;
}

bool stop_server(server_t * server)
{
	// Close the connection
	if (close(server->sock) == -1) {
		perror("Could not close socket");
		return false;
	}

	return true;
}

static void call_handler(void (* handler)(int), const connection_t * conn)
{
	handler(conn);
}

// Called by tick_server upon each new connection found
static void new_connection(
		server_t * server,
		int sock,
		const struct sockaddr_in * addr,
		soclen_t len)
{
	// Our handshake response does not change
	static iiag_handshake_t my_handshake = {
		IIAG_MARKER,
		IIAG_PROTO_MAJOR,
		IIAG_PROTO_MINOR
	};

	// Recieve the handshake from the client
	iiag_handler_t recv_handshake;
	int ret = recv(sock, &recv_handshake, sizeof(recv_handshake), 0);

	if (ret != sizeof(recv_handshake)) {
		// Bad first packet (wrong size), close the connection
		close(sock);
		return;
	}

	if (memcmp(IIAG_MARKER, recv_handshake->marker, IIAG_MARKER_LENGTH)) {
		// Not iiag protocol, close connection
		close(sock);
		return;
	}

	if (recv_handshake->major_ver != IIAG_PROTO_MAJOR) {
		// Difference in major versions, close connection
		// TODO send bad version packet?
		close(sock);
		return;
	}

	// Send our response
	if (send(sock, &my_handshake, sizeof(my_handshake), 0) != sizeof(my_handshake)) {
		// Could not send response handshake, ok, close connection
		close(sock);
		return;
	}

	// Create the connection object
	connection_t * conn = malloc(sizeof(connection_t));
	conn->sock = sock;
	conn->major_ver = recv_handshake->major_ver;
	conn->minor_ver = recv_handshake->minor_ver;

	// Trigger all on connect handlers
	list_foreach(server->connect_handlers, conn, call_handler);

	// Store the connection in the server
	list_push(server->connections, conn);
}

void tick_server(server_t * server)
{
	int ret, accepted;
	struct sockaddr_in addr;
	socklen_t socklen;

	// Accept all pending connections
	do {
		socklen = sizeof(struct sockaddr_in);
		ret = accept(server->sock, (struct sockaddr *) &addr, &socklen, SOCK_NONBLOCK);

		accepted = (ret == EAGAIN || ret == EWOULDBLOCK);

		if (accepted) {
			new_connection(server, ret, &addr, socklen);
		}
	} while (accepted);
}

void on_connect(server_t * server, bool (* handler)(int identifier))
{
	list_push(server->connect_handlers, handler);
}

void on_disconnect(server_t * server, void (* handler)(int identifier))
{
	list_push(server->disconnect_handlers, handler);
}
