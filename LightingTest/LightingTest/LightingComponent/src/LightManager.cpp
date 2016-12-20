#include "LightManager.h"
#include <iostream>

LightManager * LightManager::m_instance = nullptr;

LightManager::LightManager() {
	
}

LightManager * LightManager::getInstance() {
	if (m_instance == nullptr) {
		m_instance = new LightManager();
	}
	return m_instance;
}

bool LightManager::init(int width, int height, SDL_Renderer * renderer) {
	m_surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	if (m_surface == NULL)
	{
		std::cout << "Could not create screen: " << SDL_GetError() << std::endl;
		return false;
	}

	m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (m_surface == NULL)
	{
		std::cout << "Could not create texture: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

	return true;
}

void LightManager::update() {
	void * pixels;

	SDL_LockTexture(m_texture, &m_surface->clip_rect, &pixels, &m_surface->pitch);

	memcpy(pixels, m_surface->pixels, m_surface->w * m_surface->h);

	Uint32 * upixels = static_cast<Uint32 *>(pixels);
	for (int i = 0; i < 800 * 600; i++) {
		float distance = (i % 800) * (i % 800) + (i / 800) * (i / 800);
		//std::cout << distance << std::endl;
		if (distance == 0)
		{
			distance = 1;
		}
		upixels[i] = SDL_MapRGBA(m_surface->format, 0, 0, 0, distance < 160000 ? (distance / 160000) * 255 : 255);
	}

	SDL_UnlockTexture(m_texture);
}

void LightManager::render(SDL_Renderer * renderer) {
	SDL_RenderCopy(renderer, m_texture, NULL, NULL);
}
