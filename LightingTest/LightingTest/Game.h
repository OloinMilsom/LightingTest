#pragma once
#include "SDL2\SDL.h"
#include "LightManager.h"
#include "GameObject.h"

class Game {
private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	Light * l1;
	Light * l2;

	GameObject go1;
	GameObject go2;

	SDL_Rect back;

	float intensity;
public:
	Game();
	bool initSDL(int width, int height, const char * title);
	void loop();
};