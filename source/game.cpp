#include "game.h"
#include "gfx.h"

void Game :: Init(Gfx _gfx)
{
  gfx = _gfx;

  srand(time(NULL));

  getBlock(0);

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

void Game :: Update()
{
}

void Game :: Render()
{
  // render current block
  printf("\x1b[4;1H(%d,%d)", curr_block.tx, curr_block.ty);

  for(int py = 0; py < 4; py++) {
    for(int px = 0; px < 4; px++) {
        if(curr_block.tiles[px][py] >= 0) {
          int tx = ((curr_block.tx + px) * TILE_SIZE) + TILES_X_OFFSET;
          int ty = (curr_block.ty + py) * TILE_SIZE;
          gfx.DrawSprite(tx, ty, TILE_SIZE, TILE_SIZE, curr_block.tiles[px][py]);
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

void Game :: getBlock(int id) {
	int color = rand() & 3;
	curr_block.tx = 0;
	curr_block.ty = 0;

	switch (id) {
		case 0:
			curr_block.tiles[0][0] = -1;
			curr_block.tiles[0][1] = -1;
			curr_block.tiles[0][2] = -1;
      curr_block.tiles[0][3] = -1;
			curr_block.tiles[1][0] = -1;
			curr_block.tiles[1][1] = -1;
			curr_block.tiles[1][2] = -1;
      curr_block.tiles[1][3] = -1;
			curr_block.tiles[2][0] = -1;
			curr_block.tiles[2][1] = -1;
			curr_block.tiles[2][2] = color;
      curr_block.tiles[2][3] = color;
      curr_block.tiles[3][0] = -1;
      curr_block.tiles[3][1] = -1;
      curr_block.tiles[3][2] = color;
      curr_block.tiles[3][3] = color;
		break;
	}
}

void Game :: Left() {
  curr_block.tx -= 1;
}

void Game :: Right() {
  curr_block.tx += 1;
}
