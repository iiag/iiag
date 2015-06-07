#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#include "server.h"
#include "connection.h"
#include "iiag_proto.h"
#include "util/list.h"
#include "util/assert_do.h"

typedef struct client client_t;

struct client {
	conn_t *conn;
	int major_ver;
	int minor_ver;
};

struct server {
	char *ip;
	int port;

	int running;
	pthread_t listener;
	pthread_t dispatcher;

	list_t *clients;
	pthread_mutex_t clients_mutex;
};

// Called on every new client connected from the listener thread
static void client_connected(server_t *s, conn_t *cn) {
	// Our handshake response does not change
	static const iiag_handshake_t my_handshake = {
		IIAG_MARKER,
		IIAG_PROTO_MAJOR,
		IIAG_PROTO_MINOR
	};

	// Recieve the handshake from the client
	iiag_handshake_t recv_handshake;

	if (!conn_recv_to(cn, recv_handshake)) {
		// Could not get good first packet, close the connection
		conn_close(cn);
		return;
	}

	if (memcmp(IIAG_MARKER, recv_handshake.marker, IIAG_MARKER_LENGTH)) {
		// Not iiag protocol, close connection
		conn_close(cn);
		return;
	}

	if (recv_handshake.major_ver != IIAG_PROTO_MAJOR) {
		// Difference in major versions, close connection
		// TODO send bad version packet?
		conn_close(cn);
		return;
	}

	// Send our response
	if (!conn_send_from(cn, my_handshake)) {
		// Could not send response handshake, ok, close connection
		conn_close(cn);
		return;
	}

	// Create the client object
	client_t *cl = malloc(sizeof * cl);
	cl->conn = cn;
	cl->major_ver = recv_handshake.major_ver;
	cl->minor_ver = recv_handshake.minor_ver;

	pthread_mutex_lock(&s->clients_mutex);
	list_push(s->clients, cl);
	pthread_mutex_unlock(&s->clients_mutex);
}

// This function runs in a seperate thread to listen for new connections
static void *run_listener(void *s0) {
	server_t *s = s0;

	conn_t *ear = conn_listen(s->ip, s->port);
	conn_t *swab;

	while (s->running) {
		// Handle all new connections
		while ( (swab = conn_accept(ear)) ) {
			client_connected(s, swab);
		}

		// No need to by cycle greedy
		assert_do(!sched_yield());
	}

	return NULL;
}

// This thread handles dispatching messages to clients
static void *run_dispatcher(void *s0) {
	server_t *s = s0;

	while (s->running) {
		// TODO

		// No need to by cycle greedy
		assert_do(!sched_yield());
	}

	return NULL;
}

server_t *server_start(const char *ip, int port) {
	server_t *s = malloc(sizeof * s);

	s->ip = strdup(ip);
	s->port = port;

	s->clients = list_new();
	pthread_mutex_init(&s->clients_mutex, NULL);

	// Spin up the connection listener thread
	s->running = 1;

	assert_do(!pthread_create(&s->listener, NULL, run_listener, s));
	assert_do(!pthread_create(&s->dispatcher, NULL, run_dispatcher, s));

	return s;
}

// Called from list_foreach in server_shutdown
static void client_cleanup(void *cl0, void *_) {
	client_t *cl = cl0;
	conn_close(cl->conn);
	free(cl);
}

void server_shutdown(server_t *s) {
	// Spin down all threads
	s->running = 0;
	pthread_join(s->listener, NULL);
	pthread_join(s->dispatcher, NULL);

	list_foreach(s->clients, NULL, client_cleanup);
	list_destroy(s->clients);

	free(s->ip);
	free(s);
}
