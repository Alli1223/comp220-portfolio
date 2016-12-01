#include "stdafx.h"
#include "Voxel.h"


Voxel::Voxel()
{
}


Voxel::~Voxel()
{
}

Voxel::Voxel(long int x, long int y, long int z)
{
	setVoxelPosition(glm::vec3(x, y, z));
}