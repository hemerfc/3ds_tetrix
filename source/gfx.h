#ifndef GFX_H
#define GFX_H

#include <citro3d.h>

// Used to transfer the final rendered display to the framebuffer
#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

// Used to convert textures to 3DS tiled format
// Note: vertical flip flag set so 0,0 is top left of texture
#define TEXTURE_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define CLEAR_COLOR   0x000000FF
#define MAX_SPRITES   256

//VBO entry
typedef struct {
	float x, y, z;
	float u, v;
} VBOEntry;

class Gfx
{
     private:
          VBOEntry *vbo;
					DVLB_s* vshader_dvlb;
					shaderProgram_s program;
					int uLoc_projection;
					C3D_Mtx projection;
					size_t sprite_idx = 0; // add 1 to each sprite, reset after render
					size_t numSprites = 256;
					C3D_Tex spritesheet_tex;
					C3D_RenderTarget* target;
     public:
          void Init();
          void Render();
					void Destroy();
          size_t DrawSprite(float x, float y, int width, int height, int image );
};

#endif
