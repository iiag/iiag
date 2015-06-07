#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "net/server.h"
#include "net/connection.h"
#include "net/iiag_proto.h"

void handshake(conn_t *c) {

	iiag_handshake_t their_hs;
	iiag_handshake_t my_hs = {
		IIAG_MARKER,
		IIAG_PROTO_MAJOR,
		IIAG_PROTO_MINOR
	};

	conn_send_from(c, my_hs);
	conn_recv_to(c, their_hs);

	assert(!memcmp(my_hs.marker, their_hs.marker, IIAG_MARKER_LENGTH));
	assert(my_hs.major_ver == their_hs.major_ver);
	assert(my_hs.minor_ver == their_hs.minor_ver);
}

int main() {

	server_t *s = server_start("127.0.0.1", 1337);
	sleep(1);
	conn_t *c = conn_connect("127.0.0.1", 1337);

	handshake(c);

	conn_close(c);
	server_shutdown(s);

	return 0;
}
