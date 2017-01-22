#pragma once
#include "SDL.h"

struct RGB {
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

class Light {
	friend class LightManager;
private:
	Light();
	bool calculatePixelValue(Uint32 * upixels);

	const Uint8 MIN_ALPHA = 100;
	SDL_Point m_pos;
	Uint16 m_intensity;
	float m_falloff;
	RGB m_col;
public:
	void setPos(int x, int y);
	void setPos(SDL_Point point);
	void setIntensity(Uint16 intensity);
	void setFalloff(float falloff);
	void setColour(Uint8 r, Uint8 g, Uint8 b);
	void setColour(RGB col);
};