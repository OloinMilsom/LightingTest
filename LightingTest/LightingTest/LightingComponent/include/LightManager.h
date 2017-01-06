#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "Light.h"
#include "ShadowCaster.h"

class LightManager {
private:
	static LightManager * m_instance;
	SDL_Surface * m_surface;
	SDL_Texture * m_texture; 
	std::vector<Light *> m_lights;
	std::vector<ShadowCaster *> m_shadowCasters;

	// temp visualisation of rays
	std::vector<Ray> m_rays;

	bool m_isAmbient;
	Uint8 m_ambientLight;

	LightManager();
	void calculateRays(SDL_Point lightPoint);
public:
	static LightManager * getInstance();
	bool init(int width, int height, SDL_Renderer * renderer);
	void update();
	void render(SDL_Renderer * renderer);
	void setAmbient(bool val);
	void setAmbientIntensity(Uint8 val);
	Light * addLight();
	ShadowCaster * addShadowObject();
};