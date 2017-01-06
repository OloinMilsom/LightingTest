#pragma once
#include "SDL.h"
#include <vector>

struct Ray {
	SDL_Point pos;
	SDL_Point dir;
};

class ShadowCaster {
	friend class LightManager;
private:
	ShadowCaster();
	float getMinTVal(Ray ray);
	SDL_Point m_pos;
	std::vector<SDL_Point> m_vertices;
public:
	void setPos(int x, int y);
	void setPos(SDL_Point point);
	void addVertex(int x, int y);
	void addVertex(SDL_Point vertex);
};