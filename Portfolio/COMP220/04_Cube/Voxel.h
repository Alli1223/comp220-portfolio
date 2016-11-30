#pragma once
#include "Mesh.h"

class Voxel
{
public:
	Voxel();
	~Voxel();

	//! Voxel position
	Voxel(float x, float y, float z);

	//! Stores whether the voxel is a voxel( to be expanded upon for more bools)
	bool isVoxel = true;

	//! Gets voxel size
	float getVoxelSize() { return voxelSize; }

	//! gets and sets position
	glm::vec3 getvoxelPosition() { return voxelPosition;  }
	glm::vec3 setVoxelPosition(glm::vec3 newVoxelPosition) { return voxelPosition = newVoxelPosition; }
	
	//! Place voxel function
	void placeVoxel(Mesh& meshTexture, Mesh& snowTexture, glm::vec3 voxelPosition);

	//! Spawn Voxel
	void spawnVoxel(Mesh& meshTexture, Mesh& snowTexture, glm::vec3& voxelPosition, glm::vec3& previousVoxelPosition);

private:
	glm::vec3 voxelPosition;
	const float voxelSize = 0.5;
};

