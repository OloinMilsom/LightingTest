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

	m_boundary = new ShadowCaster();
	m_boundary->setPos(0, 0);
	m_boundary->addVertex(0, 0);
	m_boundary->addVertex(width, 0);
	m_boundary->addVertex(width, height);
	m_boundary->addVertex(0, height);

	return true;
}

void LightManager::calculateTriangles(SDL_Point lightPoint, std::vector<Polygon> * polys, Light * light) {
	std::vector<Ray> rays;
	if (!m_shadowCasters.empty()) {
		for (int casterIndex = 0; casterIndex < m_shadowCasters.size(); casterIndex++) {
			Polygon poly;

			for (int j = 0; j < m_shadowCasters[casterIndex]->m_vertices.size(); j++) {
				rays.push_back({ lightPoint,{ (m_shadowCasters[casterIndex]->m_pos.x + m_shadowCasters[casterIndex]->m_vertices[j].x - lightPoint.x), (m_shadowCasters[casterIndex]->m_pos.y + m_shadowCasters[casterIndex]->m_vertices[j].y - lightPoint.y) } });
			}
			for (int i = 0; i < m_boundary->m_vertices.size(); i++) {
				rays.push_back({ lightPoint,{ (m_boundary->m_pos.x + m_boundary->m_vertices[i].x - lightPoint.x), (m_boundary->m_pos.y + m_boundary->m_vertices[i].y - lightPoint.y) } });
			}

			std::sort(rays.begin(), rays.end());

			int prevCasterId = -1;
			Ray prevRay({}, {});
			for (int i = 0; i < rays.size(); i++) {
				m_boundary->getMinTVal(&rays[i], 0);
				m_shadowCasters[casterIndex]->getMinTVal(&rays[i], casterIndex + 1);
				rays[i].sortFactors();

				if (prevCasterId != rays[i].getFactor(0).second) {
					bool found = false;
					for (int j = 0; j < rays[i].noOfFactors(); j++) {
						if (rays[i].getFactor(j).second == prevCasterId) {
							SDL_Point p1 = rays[i].endPointByFactor(rays[i].getFactor(j).first);
							poly.xs.push_back(p1.x);
							poly.ys.push_back(p1.y);
							break;
						}
					}
					if (!found) {
						for (int j = 0; j < prevRay.noOfFactors(); j++) {
							if (prevRay.getFactor(j).second == rays[i].getFactor(0).second) {
								SDL_Point p1 = prevRay.endPointByFactor(prevRay.getFactor(j).first);
								poly.xs.push_back(p1.x);
								poly.ys.push_back(p1.y);
								break;
							}
						}
					}
				}

				SDL_Point p = rays[i].endPointByFactor(rays[i].getFactor(0).first);
				poly.xs.push_back(p.x);
				poly.ys.push_back(p.y);
				prevCasterId = rays[i].getFactor(0).second;
				prevRay = rays[i];
			}

			bool found = false;
			for (int j = 0; j < rays[0].noOfFactors(); j++) {
				if (rays[0].getFactor(j).second == prevCasterId) {
					if (rays[0].getFactor(j).second == prevCasterId) {
						SDL_Point p1 = rays[0].endPointByFactor(rays[0].getFactor(j).first);
						poly.xs.push_back(p1.x);
						poly.ys.push_back(p1.y);
						break;
					}
				}
				if (!found) {
					for (int j = 0; j < prevRay.noOfFactors(); j++) {
						if (prevRay.getFactor(j).second == rays[0].getFactor(0).second) {
							SDL_Point p1 = prevRay.endPointByFactor(prevRay.getFactor(j).first);
							poly.xs.push_back(p1.x);
							poly.ys.push_back(p1.y);
							break;
						}
					}
				}
			}
			rays.clear();
			polys->push_back(poly);
		}
	}
	else {
		Polygon poly;
		poly.xs.push_back(0);
		poly.xs.push_back(m_surface->w);
		poly.xs.push_back(m_surface->w);
		poly.xs.push_back(0);
		poly.ys.push_back(0);
		poly.ys.push_back(0);
		poly.ys.push_back(m_surface->h);
		poly.ys.push_back(m_surface->h);
		polys->push_back(poly);
	}
}

void LightManager::update() {
	for (int j = 0; j < m_lights.size(); j++) {
		m_lights[j]->calculatePixelValue(m_surface);
	}
	m_recalculatePolys = false;
}

void LightManager::render(SDL_Renderer * renderer) {
	SDL_SetSurfaceBlendMode(m_surface, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
	SDL_RenderCopy(renderer, m_texture, NULL, NULL);
	SDL_DestroyTexture(m_texture);

	SDL_FillRect(m_surface, NULL, SDL_MapRGBA(m_surface->format, 0, 0, 0, m_isAmbient ? m_ambientLight : 255));
}

void LightManager::setAmbient(bool val) {
	m_isAmbient = val;
}

void LightManager::setAmbientIntensity(Uint8 val) {
	m_ambientLight = 255 - val;
	for (int i = 0; i < m_lights.size(); i++) {
		m_lights[i]->setMinAlpha(m_ambientLight / 3);
	}
}

Light * LightManager::addLight() {
	Light * l = new Light(m_surface->w, m_surface->h, m_ambientLight / 3);
	m_lights.push_back(l);
	return l;
}

void LightManager::deleteLight(Light * light) {
	m_lights.erase(std::remove(m_lights.begin(), m_lights.end(), light), m_lights.end());
	delete light;
}

ShadowCaster * LightManager::addShadowObject() {
	ShadowCaster * s = new ShadowCaster();
	m_shadowCasters.push_back(s);
	return s;
}

void LightManager::deleteShadowObject(ShadowCaster * shadowCaster) {
	m_shadowCasters.erase(std::remove(m_shadowCasters.begin(), m_shadowCasters.end(), shadowCaster), m_shadowCasters.end());
	delete shadowCaster;
}
