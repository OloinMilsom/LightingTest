#pragma once
#include "SDL.h"
#include "LightManager.h"

class Game {
private:
	SDL_Window * window;
	SDL_Renderer * renderer;
public:
	Game();
	bool initSDL(int width, int height, const char * title);
	void loop();
};