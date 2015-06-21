#include "tile_type.h"
#include <string.h>
#include <stdlib.h>

map_t* theTiles=NULL;


int main(){

   load_tiles();
   unload_tiles();

}


void add_tile_type(char** line, int count){
   if(count<5){
      printf("Error Tile info incomplete?");
      return;
   }

   tile_type_t* tile = malloc(sizeof(tile_type_t));
   tile->type=atoi(line[0]);
   tile->passible=atoi(line[1]);
   tile->transparent=atoi(line[2]);
   //TODO implement hook loading

   map_add(theTiles,tile->type,tile);

}



void load_tiles(){

   theTiles = map_new();

   FILE* in = fopen("../data/tiles.csv","r");
   if(!in){
      printf("Failed to open tiles.csv");
      return;
   }

   int c=0;
   char** line;

   while(line = get_csv_line(in,&c)){
      if(line[0][0]!='#')
         add_tile_type(line,c);
      for(int x=0;x<c;x++)
         free(line[x]);
      free(line);
   }   


   fclose(in);
}

char** get_csv_line(FILE* f,int *num){
   *num=0;
   if(!f)
      return NULL;

   char tmp[1024];
   if(!fgets(tmp,1024,f))
      return NULL;

   int c=1;
   for(int x=0;x<1024;x++)
      if(tmp[x]==','){
         c++;
         tmp[x]='\t';
      }

   int offset=0;
   char** out;
   out = malloc(sizeof(char*)*c);

   for(int x=0;x<c;x++){
      out[x]=malloc(128);
      sscanf(tmp+offset,"%s",out[x]);
      offset+=strlen(out[x])+1;

   }

   *num=c;
   return out;
}

void free_tile_type(int i, void* tile, void* arg){
   free(tile);
}

void unload_tiles(){
   map_map(theTiles,free_tile_type,NULL);
   map_free(theTiles);
   theTiles=NULL;
}
