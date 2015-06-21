#ifndef TILE_TYPE_H
#define TILE_TYPE_H
#include "util/map.h"

typedef struct TileType tile_type_t;

struct TileType{

int type;
char passible;
char transparent;

//void (*on_use)(crtr*, zone_t*,int x, int y);
//void (*on_step)(crtr*, zone_t*,int x, int y);

};

extern map_t* theTiles;

void load_tiles();
char** get_csv_line(FILE*,int*);

#endif
