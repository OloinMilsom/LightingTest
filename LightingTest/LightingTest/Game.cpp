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

	sr1 = { 0, 0, 800, 600 };
	sr2 = { rand() % 700, rand() % 500, 100, 100 };

	LightManager::getInstance()->init(width, height, renderer);
	l1 = LightManager::getInstance()->addLight();
	l1->setIntensity(500);
	l1->setFalloff(2);
	l1->setColour(40, 20, 130);

	//Light * l2 = LightManager::getInstance()->addLight();	
	//l2->setPos(600, 300);
	//l2->setIntensity(300);
	//l2->setFalloff(2);
	//l2->setColour(255, 20, 130);

	s1 = LightManager::getInstance()->addShadowObject();
	s1->setPos(sr2.x, sr2.y);
	s1->addVertex(0, 0);
	s1->addVertex(100, 0);
	s1->addVertex(100, 100);
	s1->addVertex(0, 100);

	LightManager::getInstance()->setAmbient(true);
	LightManager::getInstance()->setAmbientIntensity(10);

	return true;
}

void Game::loop() {
	float counter = 0;
	while (true) {

		//LightManager::getInstance()->setAmbientIntensity(counter);
		//counter += 0.1f;
		l1->setPos(400 + 100 * cos(counter), 300 + 100 * sin(counter));

		s1->setPos(sr2.x, sr2.y);
		LightManager::getInstance()->update();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderFillRect(renderer, &sr1);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &sr2);

		LightManager::getInstance()->render(renderer);

		SDL_RenderPresent(renderer);
	}
}