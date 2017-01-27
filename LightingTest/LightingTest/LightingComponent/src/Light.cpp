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

Uint32 noneFunc(Uint32 dest, Uint32 source, SDL_PixelFormat * destFormat, SDL_PixelFormat * sourceFormat) {
	Uint8 sr, sg, sb, sa;
	SDL_GetRGBA(source, sourceFormat, &sr, &sg, &sb, &sa);

	return SDL_MapRGBA(destFormat, sr, sg, sb, sa);
}

Light::Light(int width, int height, Uint8 minAlpha) 
	:m_minAlpha(minAlpha),
	m_pos({ width / 2, height / 2 }),
	m_intensity(100),
	m_falloff(2),
	m_col({ 255,255,255 }) {

	m_surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	m_lightSurface = SDL_CreateRGBSurface(0, m_intensity * 2, m_intensity * 2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	m_destSurface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);	
}

bool Light::calculatePixelValue(SDL_Surface * destSurface) {
	m_polys.clear();
	m_polys.clear();
	LightManager::getInstance()->calculateTriangles(m_pos, &m_polys, this);

	SDL_SetSurfaceBlendMode(m_lightSurface, SDL_BlendMode::SDL_BLENDMODE_NONE);
	SDL_Rect lRect{ m_pos.x - m_intensity, m_pos.y - m_intensity, m_lightSurface->w , m_lightSurface->h };
	SDL_BlitSurface(m_lightSurface, NULL, m_surface, &lRect);

	SDL_Rect clearRect{ 0,0,m_surface->w, m_surface->h };
	if (m_polys.size() > 1) {
		for (int i = 0; i < m_polys.size() - 1; i++) {
			SDL_FillRect(m_destSurface, &clearRect, 0x00000000);
			texturedPolygon(m_destSurface, &m_polys[i].xs[0], &m_polys[i].ys[0], m_polys[i].xs.size(), m_surface, 0, 0, noneFunc);
			SDL_SetSurfaceBlendMode(m_destSurface, SDL_BlendMode::SDL_BLENDMODE_NONE);
			SDL_BlitSurface(m_destSurface, NULL, m_surface, NULL);
		}
		texturedPolygon(destSurface, &m_polys[m_polys.size() - 1].xs[0], &m_polys[m_polys.size() - 1].ys[0], m_polys[m_polys.size() - 1].xs.size(), m_destSurface, 0, 0, blendFunc);
	}
	else {
		texturedPolygon(destSurface, &m_polys[0].xs[0], &m_polys[0].ys[0], m_polys[0].xs.size(), m_surface, 0, 0, blendFunc);
	}

	SDL_FillRect(m_surface, NULL, 0);
	return true;
}

void Light::recalculateLight() {
	Uint32 * upixels = static_cast<Uint32 *>(m_lightSurface->pixels);
	for (int i = 0; i < m_lightSurface->w * m_lightSurface->h; i++) {
		int x = i % m_lightSurface->w;
		int y = i / m_lightSurface->w;
		// distance from centre
		float distanceSqr = (x - m_intensity) * (x - m_intensity) + (y - m_intensity) * (y - m_intensity);
		if (distanceSqr < m_intensity * m_intensity) {
			Uint8 r, g, b, a;
			r = g = b = a = 0;
			float scale = 0;
			if (m_falloff == 2) {
				float distance = sqrt(distanceSqr);
				scale = (m_intensity * m_intensity - 2 * distance * m_intensity + distanceSqr) / (m_intensity * m_intensity);
			}
			else {
				float distance = sqrt(distanceSqr);
				scale = pow(m_intensity - distance, m_falloff) / pow(m_intensity, m_falloff);
			}
			float alphaSub = (LightManager::m_instance->m_ambientLight - m_minAlpha) * scale;
			a = alphaSub;
			r = m_col.r * scale;
			g = m_col.g * scale;
			b = m_col.b * scale;
			upixels[i] = SDL_MapRGBA(m_lightSurface->format, r, g, b, a);
		}
	}
}

void Light::setPos(int x, int y) {
	m_pos = { x, y };
}

void Light::setPos(SDL_Point point) {
	m_pos = point;
}

void Light::setIntensity(Uint16 intensity) {
	m_intensity = intensity;
	SDL_FreeSurface(m_lightSurface);
	m_lightSurface = SDL_CreateRGBSurface(0, m_intensity * 2, m_intensity * 2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	recalculateLight();
}

void Light::setFalloff(float falloff) {
	m_falloff = falloff;
	recalculateLight();
}

void Light::setColour(Uint8 r, Uint8 g, Uint8 b) {
	m_col = { r, g, b };
	recalculateLight();
}

void Light::setColour(RGB col) {
	m_col = col;
	recalculateLight();
}

void Light::setMinAlpha(Uint8 alpha) {
	m_minAlpha = alpha;
	recalculateLight();
}
