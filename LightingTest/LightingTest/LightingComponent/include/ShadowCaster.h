#pragma once
#include "SDL2\SDL.h"
#include <vector>
#include <queue>
#include "Ray.h"

class ShadowCaster {
	friend class LightManager;
private:
	ShadowCaster();
	void getMinTVal(Ray * ray, int id) const;
	SDL_Point m_pos;
	std::vector<SDL_Point> m_vertices;
public:
	void setPos(int x, int y);
	void setPos(SDL_Point point);
	void addVertex(int x, int y);
	void addVertex(SDL_Point vertex);
};