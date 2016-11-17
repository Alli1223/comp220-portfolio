#include "stdafx.h"
#include "PhysicsObject.h"


PhysicsObject::PhysicsObject()
{
}


PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::tick(float deltaTime, float floorPos)
{
	/* Update position
	position += deltaTime * velocity;

	// Apply collision response
	if (position.y - 1.0f < floorPos && velocity.y < 0) // I'm lazy :(
	{
		velocity.y *= -0.8f;
	}

	// Apply gravity
	glm::vec3 acceleration(0, -9.81f, 0);

	// Update velocity
	velocity += deltaTime * acceleration;
	*/
}