/*
Copyright (c) 2013, silverhawk
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
(ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "bin.h"

void blit(Display *screen, int x, int y, uint8 color1, uint8 color2)
{
#if HAVE_LIBSDL
	blit_sdl(screen,x,y,color1*16+color2);
#else
	/* do nothing */
	1;
#endif

}

static void blit_sdl(Display *screen, int x, int y, uint32 color)
{
	uint32 *ptr = (uint32*)screen->pixels;
	ptr[x+y*SCREEN_SIZE_SDL] = color;
}

void display_init(Display *screen, int tilew, int tileh)
{
#if HAVE_LIBSDL
	display_init_wh(&tilew, &tileh);	

	SDL_Init(SDL_INIT_EVERYTHING);
	*screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32,SDL_DOUBLEBUF|SDL_SWSURFACE|SDL_HWPALETTE);
#else
	display_init_wh(0,0);	

	screen = (void*)0;
}

void display_init_wh(int *tilew, int *tileh/*clueless , int *scrw, int *scrh*/)
{
	TILE_HEIGHT = tilew;
	TILE_WIDTH = tileh; /* FIX can be 16 by NES rom setting */

	SCREEN_WIDTH = tilew * 32;
	SCREEN_HEIGHT = tileh * 32;
}
