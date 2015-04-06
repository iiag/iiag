#ifndef SERVE_H
#define SERVE_H

#include <stdbool.h>

typedef struct server server_t;

// Start the server, returns a server_t if successful, NULL if not
server_t * start_server(void);

// Stop the server, returns true on success, false otherwise
bool stop_server(server_t *);

// Should be called continuously to update the server
void tick_server(server_t *);

// Functions for registering connect/disconnect handlers
void on_connect(server_t *, void (* handler)(int identifier));
void on_disconnect(server_t *, void (* handler)(int identifier));

#endif
