#pragma once
#include "SDL.h"
#include "LightManager.h"

class Game {
private:
	SDL_Window * window;
	SDL_Renderer * renderer;
	Light * l1;
	ShadowCaster * s1;

	SDL_Rect sr1;
	SDL_Rect sr2;
public:
	Game();
	bool initSDL(int width, int height, const char * title);
	void loop();
};