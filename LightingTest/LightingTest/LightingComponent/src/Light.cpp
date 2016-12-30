#include "Light.h"
#include <iostream>

Light::Light(int x, int y, Uint16 intensity, float falloff, Uint8 r, Uint8 g, Uint8 b) 
	:m_pos({ x, y }),
	 m_intensity(intensity),
	 m_falloff(falloff),
	m_col({ r,g,b }) {
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
