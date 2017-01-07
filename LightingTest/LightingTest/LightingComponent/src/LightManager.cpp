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

	ShadowCaster * s = new ShadowCaster();
	s->setPos(0, 0);
	s->addVertex(0, 0);
	s->addVertex(width, 0);
	s->addVertex(width, height);
	s->addVertex(0, height);
	m_shadowCasters.push_back(s);

	return true;
}

void LightManager::calculateRays(SDL_Point lightPoint) {
	for (int i = 0; i < m_shadowCasters.size(); i++) {
		for (int j = 0; j < m_shadowCasters[i]->m_vertices.size(); j++) {
			m_rays.push_back({ lightPoint,{ (m_shadowCasters[i]->m_pos.x + m_shadowCasters[i]->m_vertices[j].x - lightPoint.x) * 1000, (m_shadowCasters[i]->m_pos.y + m_shadowCasters[i]->m_vertices[j].y - lightPoint.y) * 1000 } });
		}
	}	
	for (int i = 0; i < m_rays.size(); i++) {
		float minT = std::numeric_limits<float>::max();
		for (int j = 0; j < m_shadowCasters.size(); j++) {
			float tentativeT = m_shadowCasters[j]->getMinTVal(m_rays[i]);
			if (tentativeT < minT && tentativeT > 0) {
				minT = tentativeT;
			}
		}
		m_rays[i].dir.x *= minT;
		m_rays[i].dir.y *= minT;
	}
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
			m_lights[j]->calculatePixelValue(i % 800, i / 800, 255 - m_ambientLight, r, g, b, a);
		}
		upixels[i] = SDL_MapRGBA(m_surface->format, r, g, b, a);
	}
	calculateRays(m_lights[0]->m_pos);
	SDL_UnlockTexture(m_texture);

}

void LightManager::render(SDL_Renderer * renderer) {
	SDL_RenderCopy(renderer, m_texture, NULL, NULL);
	for (int i = 0; i < m_rays.size(); i++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, m_rays[i].pos.x, m_rays[i].pos.y, m_rays[i].pos.x + m_rays[i].dir.x, m_rays[i].pos.y + m_rays[i].dir.y);
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	}
	m_rays.clear();
}

void LightManager::setAmbient(bool val) {
	m_isAmbient = val;
}

void LightManager::setAmbientIntensity(Uint8 val) {
	m_ambientLight = val;
}

Light * LightManager::addLight() {
	//if (m_lights.find(id) == m_lights.end()) {
	Light * l = new Light();
	m_lights.push_back(l);
	return l;
	//}
	//return false;
}

ShadowCaster * LightManager::addShadowObject() {
	ShadowCaster * s = new ShadowCaster();
	m_shadowCasters.push_back(s);
	return s;
}
