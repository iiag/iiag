#ifndef CONNECTION_H
#define CONNECTION_H

#include <stddef.h>

/*
 * This interface provides a generic network interface
 *
 * It should garuntees that packets:
 *  - arrive
 *  - are complete (no packet fragments)
 *  - arrive in order
 */

typedef struct conn conn_t;

conn_t *conn_listen(const char *on_ip, int on_port);
conn_t *conn_connect(const char *to_ip, int to_port);
conn_t *conn_accept(conn_t *from);
void conn_close(conn_t *c);
size_t conn_send(conn_t *c, const void *packet, size_t size);
size_t conn_recv(conn_t *c, void *packet, size_t size);

#define conn_send_from(CONN, FROM) \
	conn_send(CONN, &(FROM), sizeof(FROM))

#define conn_recv_to(CONN, TO) \
	conn_recv(CONN, &(TO), sizeof(TO))

#endif
