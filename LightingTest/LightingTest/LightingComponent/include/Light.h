#pragma once
#include "SDL2\SDL.h"
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
protected:

	Light(int width, int height, Uint8 minAlpha);
	bool calculatePixelValue(SDL_Surface * destSurface);
	void recalculateLight();

	Uint8 m_minAlpha;
	SDL_Point m_pos;
	Uint16 m_intensity;
	float m_falloff;
	RGB m_col;
	SDL_Surface * m_lightSurface;
	SDL_Surface * m_surface;
	SDL_Surface * m_destSurface;
	std::vector<Polygon> m_polys;
	bool m_recalculatePolys = false;
public:
	void setPos(int x, int y);
	void setPos(SDL_Point point);
	void setIntensity(Uint16 intensity);
	void setFalloff(float falloff);
	void setColour(Uint8 r, Uint8 g, Uint8 b);
	void setColour(RGB col);
	void setMinAlpha(Uint8 alpha);
};