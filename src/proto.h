#ifndef IIAG_PROTO_H
#define IIAG_PROTO_H

#include <stdint.h>

// The most current version of the protocol
#define IIAG_PROTO_MAJOR 0
#define IIAG_PROTO_MINOR 0

// Mark the protocol as the iiag protocol
// This should probably be 4/8 byte aligned
#define IIAG_MARKER "iiag"
#define IIAG_MARKER_LENGTH (sizeof(IIAG_MARKER) - 1)

// This is the first packet sent by the server to the client to begin the
// connection, the back from the server to the client to confirm. 
typedef struct {
	char marker[IIAG_MARKER_LENGTH]; // Identifies the iiag protocol
	int16_t major_ver, minor_ver;    // Identifies the protocol version
} iiag_handshake_t;

#endif
