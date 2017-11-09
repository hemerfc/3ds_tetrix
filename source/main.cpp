/*
	Simple demo of sprites using citro3d, lodepng and immediate mode.
	Citra doesn't yet emulate immediate mode so this is hw only.
*/

#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// These headers are generated by the build process
#include "gfx.h"
#include "game.h"

int main(int argc, char **argv) {
	Gfx gfx;
  gfx.Init();

	Game game;
	game.Init(gfx);

	// Main loop
	while (aptMainLoop()) {
		hidScanInput();

		// User input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)  // KEY_START, KEY_Y, KEY_UP, KEY_DOWN
			break; // break here to return to hbmenu

		// update object here
		game.Update();

		game.Render();

		printf("\x1b[10;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[11;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[12;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}

	// Deinitialize the scene
	//gfx.Destroy();

	return 0;
}
