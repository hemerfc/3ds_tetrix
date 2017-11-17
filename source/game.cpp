#include <3ds.h>
#include "game.h"
#include "gfx.h"

void Game :: Init()
{
  srand(time(NULL));
  block.vStepDuration = 0.15f;
  block.hStepDuration = 0.15f;

  newBlock();

  // generate tiles with random colors
  for(int i = 0; i < X_LENGHT; i++) {
		for(int j = 0; j < Y_LENGHT; j++) {
      if(j > Y_LENGHT/2) {
  			int n = (rand() % 7) - 3;
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
    moveBlockLeft();
    keyLeft = true;
  }
  if (kUp & KEY_DLEFT) {
    keyLeft = false;
  }

  // move the block to right
  if (kDown & KEY_DRIGHT) {
    moveBlockRight();
    keyRight = true;
  }
  if (kUp & KEY_DRIGHT) {
    keyRight = false;
  }

  // move the block to right
  if (kDown & KEY_DDOWN) {
    moveBlockDown();
    keyRight = true;
  }
  if (kUp & KEY_DDOWN) {
    keyRight = false;
  }

  // move the block to right
  if (kDown & KEY_DUP) {
    moveBlockUp();
    keyUp = true;
  }
  if (kUp & KEY_DUP) {
    keyUp = false;
  }

  // move the block if key hold
  if (keyLeft || keyRight || keyDown || keyUp)
  {
    block.leftRightHoldStart += elapsed;
    if(block.leftRightHoldStart >= (block.hStepDuration * 2) || // to start move wait two steps
       (block.leftRightHoldStart >= block.hStepDuration && block.leftRightMoveStart)) // next moves wait one step
    {
      block.leftRightMoveStart = true;
      block.leftRightHoldStart = 0; // reset start timer
      // move the block
      if (keyLeft) {
        moveBlockLeft();
      }
      if (keyRight) {
        moveBlockRight();
      }
      if (keyDown) {
        moveBlockDown();
      }
      if (keyUp) {
        moveBlockUp();
      }
    }
  }
  else {
    block.leftRightHoldStart = 0; // reset timer
    block.leftRightMoveStart = false;
  }
}

void Game :: Render(Gfx gfx)
{
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
			block.tiles[0][2] = color;
      block.tiles[0][3] = color;
      block.tiles[1][2] = color;
      block.tiles[1][3] = color;
      block.lenght = 2;
		break;
	}
}

bool Game :: checkColision() {
  // check for colisions
  for(int py = 0; py < 4; py++) {
    for(int px = 0; px < 4; px++) {
      if(block.tiles[px][py] >= 0) { // if have a tile in block
        int x = (block.tx + px);
        int y = (block.ty + py);

        if(x < 0) {
          printf("\x1b[1;1HColision: X<0\x1b[K");
          return true;
        }
        if(x >= X_LENGHT) {
          printf("\x1b[1;1HColision: X>=X_LENGHT\x1b[K");
          return true;
        }

        if(y < 0) {
          printf("\x1b[1;1HColision: Y<0\x1b[K");
          return true;
        }
        if(y >= Y_LENGHT) {
          printf("\x1b[1;1HColision: Y>=Y_LENGHT\x1b[K");
          return true;
        }

        if(tiles[x][y] >= 0) { // if tile of block, colide with other tile
          printf("\x1b[1;1HColision: TRUE\x1b[K");
          return true;
        }
      }
    }
  }

  printf("\x1b[1;1HColision: FALSE\x1b[K");
  return false;
}

void Game :: moveBlockLeft() {
  // move block to check
  block.tx = block.tx - 1;
  // check
  if(checkColision()) {
    block.tx = block.tx + 1;
  }
}

void Game :: moveBlockRight() {
  // move block to check
  block.tx = block.tx + 1;
  // check
  if(checkColision()) {
    block.tx = block.tx - 1;
  }
}

bool Game :: moveBlockDown(){
  // move block to check
  block.ty = block.ty + 1;
  checkColision();
  // check
  if(checkColision()) {
    block.ty = block.ty - 1;
    return true;
  }
  return false;
}

void Game :: moveBlockUp() {
  // move block to check
  block.ty = block.ty - 1;
  checkColision();
  // check
  if(checkColision()) {
    block.ty = block.ty + 1;
  }
}

void Game :: rotateBlockCW() { }
void Game :: rotateBlockCCW() { }
