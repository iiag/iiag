#ifndef ZONE_H
#define ZONE_H

typedef struct Zone zone_t;

zone_t* zone_new();
void    zone_free(zone_t*);


#endif
