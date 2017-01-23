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

std::vector<Triangle> LightManager::calculateTriangles(SDL_Point lightPoint) {
	std::vector<Triangle> result;
	std::vector<Ray> rays;
	for (int i = 0; i < m_shadowCasters.size(); i++) {
		for (int j = 0; j < m_shadowCasters[i]->m_vertices.size(); j++) {
			rays.push_back({ lightPoint,{ (m_shadowCasters[i]->m_pos.x + m_shadowCasters[i]->m_vertices[j].x - lightPoint.x), (m_shadowCasters[i]->m_pos.y + m_shadowCasters[i]->m_vertices[j].y - lightPoint.y) } });
		}
	}	
	std::sort(rays.begin(), rays.end());

	int prevCasterId = -1;
	float prevFactor = 1;
	Ray prevRay({}, {});
	bool found = false;
	for (int i = 0; i < rays.size(); i++) {
		for (int j = 0; j < m_shadowCasters.size(); j++) {
			m_shadowCasters[j]->getMinTVal(&rays[i], j);
		}
		rays[i].sortFactors();
		if (prevCasterId != -1) {
			for (int j = 0; j < rays[i].noOfFactors(); j++) {
				if (rays[i].getFactor(j).second == prevCasterId) {
					result.push_back({ lightPoint, prevRay.endPointByFactor(prevFactor), rays[i].endPointByFactor(rays[i].getFactor(j).first) });
					found = true;
					break;
				}
			}
			if (!found) {
				for (int j = 0; j < prevRay.noOfFactors(); j++) {
					if (prevRay.getFactor(j).second == rays[i].getFactor(0).second) {
						result.push_back({ lightPoint, prevRay.endPointByFactor(prevRay.getFactor(j).first), rays[i].endPointByFactor(rays[i].getFactor(0).first) });
						break;
					}
				}
			}
		}
		prevRay = rays[i];
		prevCasterId = rays[i].getFactor(0).second;
		prevFactor = rays[i].getFactor(0).first;
		found = false;
	}

	for (int j = 0; j < rays[0].noOfFactors(); j++) {
		if (rays[0].getFactor(j).second == prevCasterId) {
			result.push_back({ lightPoint, prevRay.endPointByFactor(prevFactor), rays[0].endPointByFactor(rays[0].getFactor(j).first) });
			found = true;
			break;
		}
	}
	if (!found) {
		for (int j = 0; j < prevRay.noOfFactors(); j++) {
			if (prevRay.getFactor(j).second == rays[0].getFactor(0).second) {
				result.push_back({ lightPoint, prevRay.endPointByFactor(prevRay.getFactor(j).first), rays[0].endPointByFactor(rays[0].getFactor(0).first) });
				break;
			}
		}
	}
	m_triangles = result;
	return result;
}

bool LightManager::insideTriangle(SDL_Point * p, Triangle * t) {
	float alpha = static_cast<float>((t->p2.y - t->p3.y)*(p->x - t->p3.x) + (t->p3.x - t->p2.x)*(p->y - t->p3.y)) /
		static_cast<float>((t->p2.y - t->p3.y)*(t->p1.x - t->p3.x) + (t->p3.x - t->p2.x)*(t->p1.y - t->p3.y));
	float beta = static_cast<float>((t->p3.y - t->p1.y)*(p->x - t->p3.x) + (t->p1.x - t->p3.x)*(p->y - t->p3.y)) /
		static_cast<float>((t->p2.y - t->p3.y)*(t->p1.x - t->p3.x) + (t->p3.x - t->p2.x)*(t->p1.y - t->p3.y));
	float gamma = 1.0f - alpha - beta;
	return alpha > 0 && beta > 0 && gamma > 0;
}

void LightManager::update() {
	void * pixels;

	SDL_Rect t;
	t.x = t.y = 0;
	t.w = 800;
	t.h = 600;
	SDL_FillRect(m_surface, &t, SDL_MapRGBA(m_surface->format, 0,0,0, m_ambientLight));

	SDL_LockTexture(m_texture, &m_surface->clip_rect, &pixels, &m_surface->pitch);

	memcpy(pixels, m_surface->pixels, m_surface->w * m_surface->h);

	Uint32 * upixels = static_cast<Uint32 *>(pixels);

	for (int i = 0; i < 800 * 600; i++) {
		upixels[i] = SDL_MapRGBA(m_surface->format, 0, 0, 0, m_isAmbient ? m_ambientLight : 255);
	}

	for (int j = 0; j < m_lights.size(); j++) {
		m_lights[j]->calculatePixelValue(upixels);
	}
	SDL_UnlockTexture(m_texture);
}

void LightManager::render(SDL_Renderer * renderer) {
	SDL_RenderCopy(renderer, m_texture, NULL, NULL);
	/*for (int i = 0; i < m_triangles.size(); i++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, m_triangles[i].p1.x, m_triangles[i].p1.y, m_triangles[i].p2.x, m_triangles[i].p2.y);
		SDL_RenderDrawLine(renderer, m_triangles[i].p2.x, m_triangles[i].p2.y, m_triangles[i].p3.x, m_triangles[i].p3.y);
		SDL_RenderDrawLine(renderer, m_triangles[i].p3.x, m_triangles[i].p3.y, m_triangles[i].p1.x, m_triangles[i].p1.y);
	}*/

}

void LightManager::setAmbient(bool val) {
	m_isAmbient = val;
}

void LightManager::setAmbientIntensity(Uint8 val) {
	m_ambientLight = 255 - val;
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
