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
  			int n = (rand() % 6);
  			tiles[i][j] = n; // rand from -1 TO 4
  			printf("%d,", n);
  		}
  }
}

void Game :: Update()
{
}

void Game :: Render()
{
	for(int py = 0; py < Y_LENGHT; py++) {
		for(int px = 0; px < X_LENGHT; px++) {

			if(tiles[px][py] > 0) {
				if(tiles[px][py] >= 0) {
				  int id = px + py * TILE_SIZE;
					int tx = (px * TILE_SIZE) + TILES_X_OFFSET;
					int ty = py * TILE_SIZE;
					gfx.DrawSprite( id, tx, ty, TILE_SIZE, TILE_SIZE, tiles[px][py]);
				}
			}
		}
	}

  gfx.Render();
}

void Game :: getBlock(int id) {
	int color = rand() & 3;
	curr_block.x = 0;
	curr_block.y = 0;

	switch (id) {
		case 0:
			curr_block.tiles[0][0] = -1;
			curr_block.tiles[0][1] = -1;
			curr_block.tiles[0][2] = -1;
			curr_block.tiles[1][0] = -1;
			curr_block.tiles[1][1] = color;
			curr_block.tiles[1][2] = color;
			curr_block.tiles[2][0] = -1;
			curr_block.tiles[2][1] = color;
			curr_block.tiles[2][2] = color;
		break;
	}
}
