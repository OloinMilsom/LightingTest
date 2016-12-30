#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "Light.h"

class LightManager {
private:
	static LightManager * m_instance;
	SDL_Surface * m_surface;
	SDL_Texture * m_texture; 
	std::vector<Light *> m_lights;
	bool m_isAmbient;
	Uint8 m_ambientLight;

	LightManager();
public:
	static LightManager * getInstance();
	bool init(int width, int height, SDL_Renderer * renderer);
	void update();
	void render(SDL_Renderer * renderer);
	void setAmbient(bool val);
	void setAmbientIntensity(Uint8 val);
	bool addLight(std::string id, int x, int y, Uint16 intensity, float falloff, Uint8 r, Uint8 g, Uint8 b);
};