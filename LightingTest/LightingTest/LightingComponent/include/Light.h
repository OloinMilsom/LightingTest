#pragma once
#include "SDL.h"

struct RGB {
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

class Light {
private:
	SDL_Point m_pos;
	Uint16 m_intensity;
	Uint8 m_falloff;
	RGB m_col;
public:
	Light(int x, int y, Uint16 intensity, Uint8 falloff, Uint8 r, Uint8 g, Uint8 b);
	bool calculatePixelValue(int x, int y, Uint8 ambient, Uint8 & r, Uint8 & g, Uint8 & b, Uint8 & a);
};