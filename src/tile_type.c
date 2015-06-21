#include "tile_type.h"
#include <stdio.h>

map_t* theTiles;







void load_tiles(){

   theTiles = map_new();

   FILE* in = fopen("data/tiles.csv","r");

   

   fclose(in);
}

char** get_csv_line(FILE* f,int *num){

   char tmp[1024];
   if(!fgets(tmp,1024,f))
      return NULL;

   int c=0;
   for(int x=0;x<1024;x++)
      if(tmp[x]==',')
         c++;

   int offset=0;
   char** out;
   out = malloc(sizeof(char*)*c);

   for(int x=0;x<c;x++){
      out[x]=malloc(128);
      sscanf(tmp+offset,"%s",out[x]);
      offset+=strlen(out[x]);

   }

   *num=c;
   return out;
}
