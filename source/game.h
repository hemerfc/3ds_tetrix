#ifndef GAME_H
#define GAME_H

#include <3ds.h>
#include <stdio.h>
#include <time.h>
#include "gfx.h"

#define X_LENGHT      10
#define Y_LENGHT      15
#define TILE_SIZE     16
#define TILES_X_OFFSET  (400/2)-((X_LENGHT*TILE_SIZE)/2) // start point of tiles
#define STEP_TIME     1000000

typedef struct {
	double tx,ty;       // x, y of block, in tiles
	int tiles[4][4];
	int width;
	int height;
} Block;

class Game
{
     private :
          int tiles[X_LENGHT][Y_LENGHT];
          Block block;
					void newBlock();
					void setBlockTiles(int id, int rot, int color);
					void solidifyBlock();
					bool checkColision();
					void moveBlockLeft();
					void moveBlockRight();
					bool moveBlockDown();
					void moveBlockUp();
					void rotateBlockCW();
					void rotateBlockCCW();
					bool keyLeft = false;
					bool keyRight = false;
					bool keyDown = false;
					bool keyUp = false;
					double vStepDuration;  // duration of each vertical step
					double hStepDuration;  // duration of each horizontal step
					double moveKeyHoldStart;
					bool moveStart;
					double gameStep;
     public :
          void Init();
          void Update(double elapsed, u32 kDown, u32 kUp, u32 kHeld);
          void Render(Gfx gfx);
};

#endif
