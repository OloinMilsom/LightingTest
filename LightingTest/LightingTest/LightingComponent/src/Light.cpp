#include "Light.h"
#include "LightManager.h"
#include <iostream>

Light::Light() {
}

bool Light::calculatePixelValue(Uint32 * upixels) {
	LightManager::getInstance()->calculateTriangles(m_pos);
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
					a - alphaSub > MIN_ALPHA ? a -= alphaSub : a = MIN_ALPHA;
					r += m_col.r * scale;
					g += m_col.g * scale;
					b += m_col.b * scale;
					upixels[i] = SDL_MapRGBA(LightManager::m_instance->m_surface->format, r, g, b, a);
					//break;
				//}
			//}
		}
	}
	return true;
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
