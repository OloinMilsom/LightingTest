#include "ShadowCaster.h"

ShadowCaster::ShadowCaster() {
}

float ShadowCaster::getMinTVal(Ray ray) {
	float result = std::numeric_limits<float>::max();
	for (int i = 0; i < m_vertices.size(); i++) {
		SDL_Point c = { ray.pos.x - m_pos.x + m_vertices[i].x, ray.pos.y - m_pos.y + m_vertices[i].y };
		SDL_Point dir;
		if (i == m_vertices.size() - 1) {
			dir.x = m_vertices[0].x - m_vertices[i].x;
			dir.y = m_vertices[0].y - m_vertices[i].y;
		}
		else {
			dir.x = m_vertices[i + 1].x - m_vertices[i].x;
			dir.y = m_vertices[i + 1].y - m_vertices[i].y;
		}
		float tentativeT = static_cast<float>(c.x * dir.y - c.y * dir.x) / static_cast<float>(ray.dir.y * dir.x - ray.dir.x * dir.y);
		if (tentativeT > 0 && tentativeT < 1 && tentativeT < result) {
			result = tentativeT;
		}
	}
	return result;
}

void ShadowCaster::setPos(int x, int y){
	m_pos = { x, y };
}

void ShadowCaster::setPos(SDL_Point point) {
	m_pos = point;
}

void ShadowCaster::addVertex(int x, int y) {
	m_vertices.push_back({ x, y });
}

void ShadowCaster::addVertex(SDL_Point vertex) {
	m_vertices.push_back(vertex);
}
