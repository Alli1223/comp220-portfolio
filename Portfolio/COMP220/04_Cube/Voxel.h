#pragma once
#include "Mesh.h"

class Voxel
{
public:
	//! Constructor
	Voxel();
	//! Destructor
	~Voxel();

	//! Voxel position
	Voxel(long int x, long int y, long int z);

	//! Stores whether the voxel is a voxel( to be expanded upon for more bools)
	bool isVoxel = true;

	//! Gets voxel size
	float getVoxelSize() { return voxelSize; }

	//! could be used to store voxel colour in future
	//glm::vec3 colour(float r = 0, float g = 0, float b = 0);

	//! gets and sets position
	glm::vec3 getvoxelPosition() { return voxelPosition;  }
	glm::vec3 setVoxelPosition(glm::vec3 newVoxelPosition) { return voxelPosition = newVoxelPosition; }

private:
	glm::vec3 voxelPosition;
	const float voxelSize = 0.5;
};

