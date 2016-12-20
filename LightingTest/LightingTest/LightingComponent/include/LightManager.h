#pragma once
#include "SDL.h"

class LightManager {
private:
	static LightManager * m_instance;
	SDL_Surface * m_surface;
	SDL_Texture * m_texture; 

	LightManager();
public:
	static LightManager * getInstance();
	bool init(int width, int height, SDL_Renderer * renderer);
	void update();
	void render(SDL_Renderer * renderer);
};