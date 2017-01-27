#pragma once
#include "LightManager.h"

class GameObject {
private:
	SDL_Rect m_rect;
	SDL_Color m_col;
	ShadowCaster * m_caster;
public:
	GameObject();
	GameObject(SDL_Rect rect, SDL_Color col);
	void applyVector(SDL_Point v);
	void render(SDL_Renderer * renderer) const;
};