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
	Uint8 r, g, b, a;
	for (int i = 0; i < 800 * 600; i++) {
		r = g = b = 0;
		m_isAmbient ? a = 255 - m_ambientLight : a = 255;
		for (int j = 0; j < m_lights.size(); j++) {
			m_lights[j]->calculatePixelValue(i % 800, i / 800, 200, r, g, b, a);
		}
		upixels[i] = SDL_MapRGBA(m_surface->format, r, g, b, a);
	}

	SDL_UnlockTexture(m_texture);
}

void LightManager::render(SDL_Renderer * renderer) {
	SDL_RenderCopy(renderer, m_texture, NULL, NULL);
}

void LightManager::setAmbient(bool val) {
	m_isAmbient = val;
}

void LightManager::setAmbientIntensity(Uint8 val) {
	m_ambientLight = val;
}

bool LightManager::addLight(std::string id, int x, int y, Uint16 intensity, Uint8 falloff, Uint8 r, Uint8 g, Uint8 b) {
	//if (m_lights.find(id) == m_lights.end()) {
		m_lights.push_back(new Light(x, y, intensity, falloff, r, g, b));
		return true;
	//}
	//return false;
}
