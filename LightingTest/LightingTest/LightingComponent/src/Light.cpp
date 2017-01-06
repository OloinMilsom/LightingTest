#include "Light.h"
#include <iostream>

Light::Light() {
}

bool Light::calculatePixelValue(int x, int y, Uint8 ambient, Uint8 & r, Uint8 & g, Uint8 & b, Uint8 & a) {
	float distance = sqrt((x - m_pos.x) * (x - m_pos.x) + (y - m_pos.y) * (y - m_pos.y));
	if (distance < m_intensity) {
		float scale = pow(m_intensity - distance, m_falloff) / pow(m_intensity, m_falloff);
		float alphaSub = ambient * scale;
		a - alphaSub > MIN_ALPHA ? a -= alphaSub : a = MIN_ALPHA;
		r += m_col.r * scale;
		g += m_col.g * scale;
		b += m_col.b * scale;
		return true;
	}
	return false;
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
