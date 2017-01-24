#include "Light.h"
#include "LightManager.h"
#include "PolyDraw.h"
#include <iostream>

Uint32 blendFunc(Uint32 dest, Uint32 source, SDL_PixelFormat * destFormat, SDL_PixelFormat * sourceFormat) {
	Uint8 dr, dg, db, da;
	Uint8 sr, sg, sb, sa;
	SDL_GetRGBA(dest, destFormat, &dr, &dg, &db, &da);
	SDL_GetRGBA(source, sourceFormat, &sr, &sg, &sb, &sa);

	return SDL_MapRGBA(destFormat, dr + sr <= 255 ? dr + sr : 255, dg + sg <= 255 ? dg + sg : 255, db + sb <= 255 ? db + sb : 255, da - sa >= 0 ? da - sa : 0);
}

Light::Light(int width, int height) {
	m_surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	
}

bool Light::calculatePixelValue(SDL_Surface * destSurface) {
	Uint32 * upixels = static_cast<Uint32 *>(m_surface->pixels);

	for (int i = 0; i < 800 * 600; i++) {
		upixels[i] = SDL_MapRGBA(m_surface->format, 0, 0, 0, 0);
	}

	m_poly.xs.clear();
	m_poly.ys.clear();
	LightManager::getInstance()->calculateTriangles(m_pos, &m_poly);

	for (int i = 0; i < 800 * 600; i++) {
		int x = i % 800;
		int y = i / 800;
		float distanceSqr = (x - m_pos.x) * (x - m_pos.x) + (y - m_pos.y) * (y - m_pos.y);
		if (distanceSqr < m_intensity * m_intensity) {
			//for (int j = 0; j < triangles.size(); j++) {
				//SDL_Point rect = { x,y };
				//if (LightManager::getInstance()->insideTriangle(&rect, &triangles[j])) {
					float distance = sqrt(distanceSqr);
					Uint8 r, g, b, a;
					SDL_GetRGBA(upixels[i], LightManager::m_instance->m_surface->format, &r, &g, &b, &a);
					//std::cout << "i: " << i << "\tr: " << static_cast<int>(r) << "\tg: " << static_cast<int>(g) << "\tb: " << static_cast<int>(b) << std::endl;
					float scale = pow(m_intensity - distance, m_falloff) / pow(m_intensity, m_falloff);
					float alphaSub = LightManager::m_instance->m_ambientLight * scale;
					a += alphaSub;
					r += m_col.r * scale;
					g += m_col.g * scale;
					b += m_col.b * scale;
					upixels[i] = SDL_MapRGBA(LightManager::m_instance->m_surface->format, r, g, b, a);
					//break;
				//}
			//}
		}
	}

	texturedPolygon(destSurface, &m_poly.xs[0], &m_poly.ys[0], m_poly.xs.size(), m_surface, 0, 0, blendFunc);

	return true;
}

Polygon Light::getPoly() const {
	return m_poly;
}

SDL_Surface * Light::getSurface() const {
	return m_surface;
}

void Light::setPos(int x, int y) {
	m_pos = { x, y };
}

void Light::setPos(SDL_Point point) {
	m_pos = point;
}

void Light::setIntensity(Uint16 intensity) {
	m_intensity = intensity;
}

void Light::setFalloff(float falloff) {
	m_falloff = falloff;
}

void Light::setColour(Uint8 r, Uint8 g, Uint8 b) {
	m_col = { r, g, b };
}

void Light::setColour(RGB col) {
	m_col = col;
}
