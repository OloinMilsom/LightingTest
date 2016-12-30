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
	const Uint8 MIN_ALPHA = 100;
	SDL_Point m_pos;
	Uint16 m_intensity;
	float m_falloff;
	RGB m_col;
public:
	Light(int x, int y, Uint16 intensity, float falloff, Uint8 r, Uint8 g, Uint8 b);
	bool calculatePixelValue(int x, int y, Uint8 ambient, Uint8 & r, Uint8 & g, Uint8 & b, Uint8 & a);
};