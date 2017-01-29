#pragma once
#include <vector>
#include <string>
#include "SDL2\SDL.h"
#include "Light.h"
#include "ShadowCaster.h"

class LightManager {
	friend class Light;
	friend class ShadowCaster;
private:
	static LightManager * m_instance;
	SDL_Surface * m_surface;
	SDL_Surface * m_shadowSurface;
	SDL_Texture * m_texture;

	std::vector<Light *> m_lights;
	std::vector<ShadowCaster *> m_shadowCasters;
	ShadowCaster * m_boundary;

	bool m_isAmbient;
	bool m_recalculatePolys = true;
	Uint8 m_ambientLight;

	LightManager();
	void calculateTriangles(SDL_Point lightPoint, std::vector<Polygon> * polys, Light * light);
public:
	static LightManager * getInstance();
	bool init(int width, int height, SDL_Renderer * renderer);
	void update();
	void render(SDL_Renderer * renderer);
	void setAmbient(bool val);
	void setAmbientIntensity(Uint8 val);
	Light * addLight();
	void deleteLight(Light * light);
	ShadowCaster * addShadowObject();
	void deleteShadowObject(ShadowCaster * shadowCaster);
};

