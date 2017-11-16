#include <3ds.h>
#include "game.h"
#include "gfx.h"

void Game :: Init()
{
  srand(time(NULL));
  block.hStepTime = 50;
  block.vStepTime = 50;

  newBlock();

  // generate tiles with random colors
  for(int i = 0; i < X_LENGHT; i++) {
		for(int j = 0; j < Y_LENGHT; j++) {
      if(j > Y_LENGHT/2) {
  			int n = (rand() % 5) - 1;
  			tiles[i][j] = n;
      }
      else
      {
        tiles[i][j] = -1;
      }
		}
  }
}

void Game :: Update(double elapsed, u32 kDown, u32 kUp, u32 kHeld)
{
  // move the block to left
  if (kDown & KEY_DLEFT) {
    block.tx -= 1;
    keyLeft = true;
  }
  if (kUp & KEY_DLEFT) {
    keyLeft = false;
  }

  // move the block to right
  if (kDown & KEY_DRIGHT) {
    block.tx += 1;
    keyRight = true;
  }
  if (kUp & KEY_DRIGHT) {
    keyRight = false;
  }

  // move the block if key hold
  if (keyLeft || keyRight)
  {
    block.hStartStep += elapsed;
    if(block.hStartStep >= block.hStepTime) // if completed one step
    {
      // move the block
      if (keyLeft) {
        printf("\x1b[5;1HKEY_DLEFT %.2f", (float)block.hStartStep);
        block.tx -= 1;
      }
      if (keyRight) {
        printf("\x1b[5;1HKEY_DRIGHT %.2f", (float)block.hStartStep);
        block.tx += 1;
      }

      block.hStartStep -= block.hStepTime; // reset step timer to diference
    }
  }
  else
    block.hStartStep = 0; // reset timer
}

void Game :: Render(Gfx gfx)
{
  // render current block
  for(int py = 0; py < 4; py++) {
    for(int px = 0; px < 4; px++) {
      if(block.tiles[px][py] >= 0) {
        int tx = ((block.tx + px) * TILE_SIZE) + TILES_X_OFFSET;
        int ty = (block.ty + py) * TILE_SIZE;
        gfx.DrawSprite(tx, ty, TILE_SIZE, TILE_SIZE, block.tiles[px][py]);
      }
    }
  }

  // render all tiles
	for(int py = 0; py < Y_LENGHT; py++) {
		for(int px = 0; px < X_LENGHT; px++) {
			if(tiles[px][py] >= 0) {
				int tx = (px * TILE_SIZE) + TILES_X_OFFSET;
				int ty = py * TILE_SIZE;
				gfx.DrawSprite( tx, ty, TILE_SIZE, TILE_SIZE, tiles[px][py]);
			}
		}
	}

  gfx.Render();
}

void Game :: newBlock() {
  //int id = rand() & 3;
	int color = rand() & 3;
	block.tx = 0.0;
	block.ty = 0.0;
  setBlockTiles(0,0,color);
}

void Game :: setBlockTiles(int id, int rot, int color) {
  for(int i=0; i < 4; i++)
    for(int j=0; j < 4; j++)
      block.tiles[i][j] = -1;

	switch (id) {
		case 0:
			block.tiles[2][2] = color;
      block.tiles[2][3] = color;
      block.tiles[3][2] = color;
      block.tiles[3][3] = color;
      block.lenght = 2;
		break;
	}
}
