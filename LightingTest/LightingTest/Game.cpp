#include "Game.h"
#include <iostream>

Game::Game() : window(NULL), renderer(NULL) {
}

bool Game::initSDL(int width, int height, const char* title) {
	int e = SDL_Init(SDL_INIT_EVERYTHING);

	if (e != 0)
	{
		// problem with SDL?...
		std::cout << "Could not init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		// In the case that the renderer could not be made...
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	LightManager::getInstance()->init(width, height, renderer);

	return true;
}

void Game::loop() {
	while (true) {
		LightManager::getInstance()->update();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect sr1 = { 0, 0, 800, 600 };
		SDL_Rect sr2 = { 100, 100, 100, 100 };
		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderFillRect(renderer, &sr1);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &sr2);

		LightManager::getInstance()->render(renderer);

		SDL_RenderPresent(renderer);
	}
}