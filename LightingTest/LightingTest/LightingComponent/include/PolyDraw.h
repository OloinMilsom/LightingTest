#pragma once
#include "SDL.h"

// modified version of SDL2_gfx texturedPolygon function that writes to a surface instead of rendering straight away
int texturedPolygon(SDL_Surface * dest, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface * texture, int texture_dx, int texture_dy, SDL_BlendMode blendMode);