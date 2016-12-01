#pragma once
#include "stdafx.h"
class Player_Movement
{
public:
	//! Constructor
	Player_Movement();
	//! Destructor
	~Player_Movement();

	//! Moves the player
	void playerMove(glm::vec4& playerPosition);

	glm::vec4 GetPlayerLook();
private:

	float playerSpeed = 0.2f;
	float speedModifierIncrease = 2.0f;
	float plX, plY, plZ, plW;
	int mouseX, mouseY;
	float playerPitch = 0;
	float playerYaw = 0;
};

