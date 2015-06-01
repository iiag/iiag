#ifndef WORLD_H
#define WORLD_H

typedef struct World world_t;

world_t* world_new();
void     world_free(world_t*);
zone_t*  world_get_zone(world_t*,int,int,int,int);




#endif
