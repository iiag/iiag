#ifndef AVL_MAP_H
#define AVL_MAP_H

typedef struct AVL_Map map_t;

typedef void (*map_map_func)(int, void*, void*);

map_t* map_new();
void   map_free(map_t*);
void*  map_get(map_t*,int);
void*  map_remove(map_t*,int);
void   map_add(map_t*,int,void*);
void   map_map(map_t*,map_map_func,void* data);





#endif

