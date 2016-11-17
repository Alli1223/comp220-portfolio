#pragma once
class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

	void tick(float deltaTime, float goundPos);
};

