#pragma once
#include "SDL.h"
#include <vector>

struct Polygon {
	std::vector<Sint16> xs;
	std::vector<Sint16> ys;
};

struct RGB {
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

class Light {
	friend class LightManager;
private:
	Light(int width, int height);
	bool calculatePixelValue(SDL_Surface * destSurface);
	Polygon getPoly() const;
	SDL_Surface * getSurface() const;

	const Uint8 MIN_ALPHA = 100;
	SDL_Point m_pos;
	Uint16 m_intensity;
	float m_falloff;
	RGB m_col;
	SDL_Surface * m_surface;
	Polygon m_poly;
public:
	void setPos(int x, int y);
	void setPos(SDL_Point point);
	void setIntensity(Uint16 intensity);
	void setFalloff(float falloff);
	void setColour(Uint8 r, Uint8 g, Uint8 b);
	void setColour(RGB col);
};