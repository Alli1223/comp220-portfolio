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

	//! Returns the players look direction
	glm::vec4 GetPlayerLook();
private:

	//! Players movement speed
	float playerSpeed = 0.2f;
	//! Players speed modifier
	float speedModifierIncrease = 2.0f;
	//! Mouse sensitivity
	float mouseSensitivity = 0.005f;

	//! Max up & down view distance
	const float maxPitch = glm::radians(89.0f);

	//! float values for playerlook
	float plX, plY, plZ, plW;
	int mouseX, mouseY;
	float playerPitch = 0;
	float playerYaw = 0;
};

