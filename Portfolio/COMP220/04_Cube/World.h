#pragma once
#include "Mesh.h"
#include "Terrain.h"
#include "Voxel.h"
#include "Perlin_Noise.h"
#include "Player_Movement.h"

class World
{
public:
	//! Consturctor
	World();
	//! Destructor
	~World();

	void createWorld();


private:
	//! Pointer to window
	SDL_Window* window;
	//! glContext
	SDL_GLContext glContext;

	//! Size of the window
	int windowWidth = 1000;
	int windowHeight = 1000;

	//! Stores the players initial starting position
	float PlayerStartX = 50, PlayerStartY = 50, PlayerStartZ = 50;

	//! How height the camera is from the terrain
	float playerheight = 3.0f;
};

