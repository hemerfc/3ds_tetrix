#ifndef GAME_H
#define GAME_H

#include <3ds.h>
#include <stdio.h>
#include "gfx.h"

#define X_LENGHT      10
#define Y_LENGHT      20
#define TILE_SIZE     16
#define TILES_X_OFFSET  (400/2)-((X_LENGHT*TILE_SIZE)/2) // start point of tiles

typedef struct {
	int x,y;
	int tiles[3][3];
} Block;

class Game
{
     private :
          int tiles[X_LENGHT][Y_LENGHT];
          Block curr_block;
          Gfx gfx;
					void getBlock(int id);
     public :
          void Init(Gfx _gfx);
          void Update();
          void Render();
};

#endif
