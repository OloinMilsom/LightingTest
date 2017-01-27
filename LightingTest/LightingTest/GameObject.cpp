#include "GameObject.h"

GameObject::GameObject() {
}

GameObject::GameObject(SDL_Rect rect, SDL_Color col) : m_rect(rect), m_col(col) {
	m_caster = LightManager::getInstance()->addShadowObject();
	m_caster->setPos(rect.x, rect.y);
	m_caster->addVertex(0, 0);
	m_caster->addVertex(0, rect.w);
	m_caster->addVertex(rect.h, rect.w);
	m_caster->addVertex(rect.h, 0);
}

void GameObject::applyVector(SDL_Point v) {
	m_rect.x += v.x;
	m_rect.y += v.y;
	m_caster->setPos({ m_rect.x, m_rect.y });
}

void GameObject::render(SDL_Renderer * renderer) const {
	SDL_SetRenderDrawColor(renderer, m_col.r, m_col.g, m_col.b, m_col.a);
	SDL_RenderFillRect(renderer, &m_rect);
}
