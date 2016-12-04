#pragma once
#include "SDL.h"
class Game {
private:
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Surface * screen;
public:
	Game();
	bool initSDL(int width, int height, const char * title);
	void loop();
};