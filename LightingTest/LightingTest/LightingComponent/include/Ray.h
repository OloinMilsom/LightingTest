#pragma once
#include "SDL.h"
#include <vector>
#include <algorithm>

class Ray {
private:
	SDL_Point m_pos;
	SDL_Point m_dir;
	std::vector<std::pair<float, int>> m_castFactors;
public:
	Ray(SDL_Point pos, SDL_Point dir);
	SDL_Point getPos() const;
	SDL_Point getDir() const;
	// gets scalar along ray a that is its intersection with ray b
	// return -1 if no valid scalar was found
	float getRayIntersect(Ray * other) const;
	void addFactor(float factor, int casterId);
	void sortFactors();
	void clearFactors();
	std::pair<float, int> getFactor(int i) const; 
	int noOfFactors();
	SDL_Point endPointByFactor(float factor);
	bool operator<(Ray & other);
};
