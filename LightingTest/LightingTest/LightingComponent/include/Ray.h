#pragma once
#include "SDL2\SDL.h"
#include <vector>
#include <algorithm>

class Ray {
private:
	SDL_Point m_pos;
	SDL_Point m_dir;
	std::vector<std::pair<float, int>> m_castFactors;
public:
	Ray(SDL_Point pos, SDL_Point dir);
	void setPos(SDL_Point pos);
	void setDir(SDL_Point dir);
	SDL_Point getPos() const;
	SDL_Point getDir() const;
	float getAngle() const;
	float getRayIntersect(Ray * other) const;
	void addFactor(float factor, int casterId);
	void sortFactors();
	void clearFactors();
	std::pair<float, int> getFactor(int i) const; 
	int noOfFactors();
	SDL_Point endPointByFactor(float factor);
	bool operator<(Ray & other);
};
