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

	screen = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	if (screen == NULL)
	{
		// In the case that the renderer could not be made...
		std::cout << "Could not create screen: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Game::loop() {
	while (true) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect sr1 = { 0, 0, 800, 600 };
		SDL_Rect sr2 = { 100, 100, 100, 100 };
		SDL_FillRect(screen, &sr1, SDL_MapRGB(screen->format, 64, 64, 64));
		SDL_FillRect(screen, &sr2, SDL_MapRGB(screen->format, 255, 0, 0));

		Uint32 * pixels = static_cast<Uint32 *>(screen->pixels);
		for (int i = 0; i < 800 * 600; i++) {
			Uint8 r = 0;
			Uint8 g = 0;
			Uint8 b = 0;
			SDL_GetRGB(pixels[i], screen->format, &r, &g, &b);
			float distance = sqrt((i % 800) * (i % 800) + (i / 800) * (i / 800)) * 0.01f;
			if (distance == 0)
			{
				distance = 1;
			}
			pixels[i] = SDL_MapRGB(screen->format, r / distance, g / distance, b / distance);
		}
		screen->pixels = pixels;

		SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, screen);

		SDL_RenderCopy(renderer, tex, NULL, NULL);

		SDL_DestroyTexture(tex);

		SDL_RenderPresent(renderer);
	}
}