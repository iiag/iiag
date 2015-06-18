#ifndef NET_SERVER_H
#define NET_SERVER_H

typedef struct server server_t;

server_t *server_start(const char *, int);
void      server_shutdown(server_t *server);

#endif
