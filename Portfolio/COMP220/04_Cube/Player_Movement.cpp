#include "stdafx.h"
#include "Player_Movement.h"


Player_Movement::Player_Movement()
{
}


Player_Movement::~Player_Movement()
{
}

void Player_Movement::playerMove(glm::vec4& playerPosition)
{
	SDL_GetRelativeMouseState(&mouseX, &mouseY);

	// Mouse sensitivity
	playerYaw -= mouseX * 0.005f;
	playerPitch -= mouseY * 0.005f;

	// Max up & down view distance
	const float maxPitch = glm::radians(89.0f);
	if (playerPitch > maxPitch)
		playerPitch = maxPitch;
	if (playerPitch < -maxPitch)
		playerPitch = -maxPitch;

	// Forwards and back movement controls
	glm::vec4 playerLook(0, 0, -1, 0);
	glm::mat4 playerRotation;
	playerRotation = glm::rotate(playerRotation, playerYaw, glm::vec3(0, 1, 0));
	playerRotation = glm::rotate(playerRotation, playerPitch, glm::vec3(1, 0, 0));
	playerLook = playerRotation * playerLook;
	plX = playerLook.x, plY = playerLook.y, plZ = playerLook.z, plW = playerLook.w;

	glm::vec4 playerForward = playerLook;


	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	if (keyboardState[SDL_SCANCODE_W])
	{
		playerPosition += playerForward * playerSpeed;
		// Speed modifier
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			playerPosition += playerForward * speedModifierIncrease;
		}
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		playerPosition -= playerForward * playerSpeed;
		// Speed modifier
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			playerPosition -= playerForward * speedModifierIncrease;
		}
	}

	// For left and right movement
	glm::vec4 playerRight(0, 0, -1, 0);
	glm::mat4 playerRightRotation;
	playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
	playerRight = playerRightRotation * playerRight;

	if (keyboardState[SDL_SCANCODE_A])
	{
		playerPosition -= playerRight * playerSpeed;
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			playerPosition -= playerRight * speedModifierIncrease;
		}
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		playerPosition += playerRight * playerSpeed;
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			playerPosition += playerRight * speedModifierIncrease;
		}
	}
}

glm::vec4 Player_Movement::GetPlayerLook()
{
	glm::vec4 returnplayerlook(plX, plY, plZ, plW);
	return returnplayerlook;
}