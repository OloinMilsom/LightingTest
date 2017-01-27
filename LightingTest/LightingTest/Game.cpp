#include "Game.h"
#include <iostream>
#include <time.h>

Game::Game() : window(NULL), renderer(NULL) {
}

bool Game::initSDL(int width, int height, const char* title) {
	int e = SDL_Init(SDL_INIT_EVERYTHING);

	srand(time(0));

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

	back = { 0, 0, width, height };

	intensity = 300;

	LightManager::getInstance()->init(width, height, renderer);
	l1 = LightManager::getInstance()->addLight();
	l1->setPos(200, 300);
	l1->setIntensity(intensity);
	l1->setFalloff(2);
	l1->setColour(0, 255, 220);

	l2 = LightManager::getInstance()->addLight();	
	l2->setPos(600, 300);
	l2->setIntensity(300);
	l2->setFalloff(2);
	l2->setColour(255, 0, 255);

	LightManager::getInstance()->setAmbient(true);
	LightManager::getInstance()->setAmbientIntensity(50);

	go1 = GameObject({ 20, 20, 100, 100 }, { 123, 45, 67 });
	go2 = GameObject({ 350, 250, 100, 100 }, { 76, 54, 32 });

	return true;
}

void Game::loop() {
	while (true) {

		SDL_Event e;

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_w:
					go1.applyVector({ 0, -10 });
					break;
				case SDLK_a:
					go1.applyVector({ -10, 0 });
					break;
				case SDLK_s:
					go1.applyVector({ 0, 10 });
					break;
				case SDLK_d:
					go1.applyVector({ 10, 0 });
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				l1->setPos(e.motion.x, e.motion.y);
				break; 
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					intensity += 50;
					if (intensity > 500) {
						intensity = 150;
					}
					l1->setIntensity(intensity);
				}
				break;
			}
		}

		LightManager::getInstance()->update();

		SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);
		SDL_RenderClear(renderer);

		go1.render(renderer);
		go2.render(renderer);

		LightManager::getInstance()->render(renderer);

		SDL_RenderPresent(renderer);
	}
}