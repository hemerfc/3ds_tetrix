#include "gfx.h"
#include <citro3d.h>

#include "lodepng.h"
#include "vshader_shbin.h"
#include "ballsprites_png.h"

struct { float left, right, top, bottom; } images[4] = {
	{0.0f, 0.5f, 0.0f, 0.5f},
	{0.5f, 1.0f, 0.0f, 0.5f},
	{0.0f, 0.5f, 0.5f, 1.0f},
	{0.5f, 1.0f, 0.5f, 1.0f},
};

void Gfx :: Init() {
  // Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	consoleInit(GFX_BOTTOM, NULL);

	// Initialize the render target
	target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

  // Load the vertex shader, create a shader program and bind it
  vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
  shaderProgramInit(&program);
  shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
  C3D_BindProgram(&program);

  // Get the location of the uniforms
  uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

  // Allocate VBO
  vbo = (VBOEntry*)linearAlloc(sizeof(VBOEntry) * 6 * MAX_SPRITES);

  // Configure attributes for use with the vertex shader
  // Attribute format and element count are ignored in immediate mode
  C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
  AttrInfo_Init(attrInfo);
  AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
  AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v2=texcoord

  // Compute the projection matrix
  // Note: we're setting top to 240 here so origin is at top left.
  Mtx_OrthoTilt(&projection, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

  // Configure buffers
  C3D_BufInfo* bufInfo = C3D_GetBufInfo();
  BufInfo_Init(bufInfo);
  BufInfo_Add(bufInfo, vbo, sizeof(VBOEntry), 2, 0x10);

  unsigned char* image;
  unsigned width, height;

  lodepng_decode32(&image, &width, &height, ballsprites_png, ballsprites_png_size);

  printf("\x1b[1;1Hwidth:      %d\x1b[K", width);
  printf("\x1b[2;1Hheight:     %d\x1b[K", height);
  printf("\x1b[3;1Hresult:     %d\x1b[K", (width*height*4));

  u8* gpusrc = (u8*)linearAlloc(width*height*4);

  // GX_DisplayTransfer needs input buffer in linear RAM
  u8* src=image;
	u8* dst=gpusrc;

  // lodepng outputs big endian rgba so we need to convert
  for(unsigned i = 0; i<width*height; i++) {
  	int r = *src++;
  	int g = *src++;
  	int b = *src++;
  	int a = *src++;

		int sd = r+g+b+a+sd;
  	*dst++ = a;
  	*dst++ = b;
  	*dst++ = g;
  	*dst++ = r;
  }

  // ensure data is in physical ram
  GSPGPU_FlushDataCache(gpusrc, width*height*4);

  // Load the texture and bind it to the first texture unit
  C3D_TexInit(&spritesheet_tex, width, height, GPU_RGBA8);

  // Convert image to 3DS tiled texture format
  C3D_SafeDisplayTransfer ((u32*)gpusrc, GX_BUFFER_DIM(width,height), (u32*)spritesheet_tex.data, GX_BUFFER_DIM(width,height), TEXTURE_TRANSFER_FLAGS);
  gspWaitForPPF();

  C3D_TexSetFilter(&spritesheet_tex, GPU_LINEAR, GPU_NEAREST);
  C3D_TexBind(0, &spritesheet_tex);

  free(image);
  linearFree(gpusrc);

  // Configure the first fragment shading substage to just pass through the texture color
  // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
  C3D_TexEnv* env = C3D_GetTexEnv(0);
  C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
  C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
  C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

  // Configure depth test to overwrite pixels with the same depth (needed to draw overlapping sprites)
  C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
}

void Gfx :: Render(void) {
	// Render the scene
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C3D_FrameDrawOn(target);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);

  // fim
	C3D_DrawArrays(GPU_TRIANGLES, 0, numSprites * 6);

	C3D_FrameEnd(0);
}

void Gfx :: Destroy(void) {
	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

	// Free the vbo
	linearFree(vbo);

	// Deinitialize graphics
	C3D_Fini();
}

//---------------------------------------------------------------------------------
void Gfx :: DrawSprite( size_t idx, float x, float y, int width, int height, int image ) {
//---------------------------------------------------------------------------------
	float left = images[image].left;
	float right = images[image].right;
	float top = images[image].top;
	float bottom = images[image].bottom;

	VBOEntry *entry = &vbo[idx*6];

	*entry++ = (VBOEntry){ x,       y,        0.5f, left,  top    };
	*entry++ = (VBOEntry){ x,       y+height, 0.5f, left,  bottom };
	*entry++ = (VBOEntry){ x+width, y,        0.5f, right, top    };

	*entry++ = (VBOEntry){ x+width, y,        0.5f, right, top    };
	*entry++ = (VBOEntry){ x,       y+height, 0.5f, left,  bottom };
	*entry++ = (VBOEntry){ x+width, y+height, 0.5f, right, bottom };
}
