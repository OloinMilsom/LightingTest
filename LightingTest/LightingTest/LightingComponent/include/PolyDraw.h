#pragma once
#include "SDL2\SDL.h"

// modified version of SDL2_gfx texturedPolygon function that writes to a surface instead of rendering straight away
// modified version was required so I could continue to edit pixel data
int texturedPolygon(SDL_Surface * dest, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface * texture, int texture_dx, int texture_dy, Uint32 (*pixelfunc)(Uint32, Uint32, SDL_PixelFormat *, SDL_PixelFormat *));