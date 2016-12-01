#pragma once
#include "Mesh.h"
#include "Terrain.h"
#include "Voxel.h"
#include "Perlin_Noise.h"

class World
{
public:
	World();
	~World();

	void createWorld();


private:

	SDL_Window* window;
	SDL_GLContext glContext;

	float playerSpeed = 0.2f;
	float speedModifierIncrease = 2.0f;
	int windowWidth = 1000;
	int windowHeight = 1000;
};

