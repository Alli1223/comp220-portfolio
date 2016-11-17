#include "stdafx.h"
#include "Physics.h"


Physics::Physics()
{
}



Physics::~Physics()
{
}


void Physics::UpdatePhysics(UINT32 lastFrameTime)
{
	// Calculate delta time
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
	lastFrameTime = currentTime;
}