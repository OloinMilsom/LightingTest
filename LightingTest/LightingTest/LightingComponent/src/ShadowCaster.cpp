#include "ShadowCaster.h"
#include "LightManager.h"

ShadowCaster::ShadowCaster() {
}

void ShadowCaster::getMinTVal(Ray * ray, int id) const {
	for (int i = 0; i < m_vertices.size(); i++) {
		SDL_Point pos = { m_pos.x + m_vertices[i].x, m_pos.y + m_vertices[i].y };
		SDL_Point dir;
		if (i == m_vertices.size() - 1) {
			dir.x = m_vertices[0].x - m_vertices[i].x;
			dir.y = m_vertices[0].y - m_vertices[i].y;
		}
		else {
			dir.x = m_vertices[i + 1].x - m_vertices[i].x;
			dir.y = m_vertices[i + 1].y - m_vertices[i].y;
		}
		Ray temp = Ray(pos, dir);
		float u = ray->getRayIntersect(&temp);
		float v = temp.getRayIntersect(ray);
		if (u >= 0 && v >= 0 && v <= 1)
		{
			ray->addFactor(u, id);
		}
	}
}

void ShadowCaster::setPos(int x, int y){
	m_pos = { x, y };
	LightManager::getInstance()->m_recalculatePolys = true;
}

void ShadowCaster::setPos(SDL_Point point) {
	m_pos = point;
	LightManager::getInstance()->m_recalculatePolys = true;
}

void ShadowCaster::addVertex(int x, int y) {
	m_vertices.push_back({ x, y });
}

void ShadowCaster::addVertex(SDL_Point vertex) {
	m_vertices.push_back(vertex);
}
