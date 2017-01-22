#include "Ray.h"

Ray::Ray(SDL_Point pos, SDL_Point dir) {
	m_pos = pos;
	m_dir = dir;
}

SDL_Point Ray::getPos() const {
	return m_pos;
}

SDL_Point Ray::getDir() const {
	return m_dir;
}

float Ray::getRayIntersect(Ray * other) const {
	if ((m_dir.x * other->m_dir.y) - (m_dir.y * other->m_dir.x) != 0) {
		return static_cast<float>((m_pos.y * other->m_dir.x) + (other->m_dir.y * other->m_pos.x) - (other->m_pos.y * other->m_dir.x) - (other->m_dir.y * m_pos.x)) / static_cast<float>((m_dir.x * other->m_dir.y) - (m_dir.y * other->m_dir.x));
	}
	return -1;
}

void Ray::addFactor(float factor, int casterId) {
	m_castFactors.push_back(std::make_pair(factor, casterId));
}

void Ray::sortFactors() {
	std::sort(m_castFactors.begin(), m_castFactors.end());
}

void Ray::clearFactors() {
	m_castFactors.clear();
}

std::pair<float, int> Ray::getFactor(int i) const {
	if (i < m_castFactors.size()) {
		return m_castFactors[i];
	}
	return std::make_pair(1, std::numeric_limits<int>::max());
}

int Ray::noOfFactors() {
	return m_castFactors.size();
}

SDL_Point Ray::endPointByFactor(float factor) {
	return{ m_pos.x + static_cast<int>(m_dir.x * factor), m_pos.y + static_cast<int>(m_dir.y * factor) };
}

bool Ray::operator<(Ray & other) {
	return atan2(m_dir.y, m_dir.x) < atan2(other.m_dir.y, other.m_dir.x);
}
