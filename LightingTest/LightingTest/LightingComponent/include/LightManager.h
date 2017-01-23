#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "Light.h"
#include "ShadowCaster.h"

struct Triangle {
	SDL_Point p1;
	SDL_Point p2;
	SDL_Point p3;
};

class LightManager {
	friend class Light;
private:
	static LightManager * m_instance;
	SDL_Surface * m_surface;
	SDL_Surface * m_shadowSurface;
	SDL_Texture * m_texture; 
	std::vector<Light *> m_lights;
	std::vector<ShadowCaster *> m_shadowCasters;

	// temp visualisation of triangles
	std::vector<Triangle> m_triangles;

	bool m_isAmbient;
	Uint8 m_ambientLight;

	LightManager();
	std::vector<Triangle> calculateTriangles(SDL_Point lightPoint);
	bool insideTriangle(SDL_Point * p, Triangle * t);
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

