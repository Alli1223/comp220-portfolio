#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
	//TODO: Delete terrain chunks after a set distance
}

void Terrain::makeGrid()
{
	// Generate perlin noise from seed
	PerlinNoise perlinNoise;
	int seed = SDL_GetTicks();
	perlinNoise.GenerateNoise(seed);

	// Temp y value before perlin noise
	float y = 0;

	// Populate the grid with Voxels
	for (float x = 0; x < terrainWidth; x++)
	{
		std::vector<Voxel> column;
		Voxels.push_back(column);

		for (float z = 0; z < terrainDepth; z++)
		{
			float terrainHeight = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), 0);
			terrainHeight = (char)((terrainHeight - noiseMin) * (255 / (noiseMax - noiseMin)));

			Voxel voxel(x, terrainHeight, z);
			Voxels[x].push_back(voxel);
			
		}
	}
}

float Terrain::getHeight(float x, float z)
{
	float height = Voxels[x][z].getvoxelPosition().y;
	return height;
}
void Terrain::generateTerrain(Mesh& groundTexture, Mesh& snowTexture)
{
	

	// Makes the grid of voxels
	makeGrid();
	glm::vec3 lastVoxelPos(0, 0, 0);

	for (float x = 0; x < terrainWidth; x++)
	{
		for (float z = 0; z < terrainDepth; z++)
		{
			
			glm::vec3 voxelPos(x + voxelSize + voxelSize, getHeight(x,z) + voxelSize + voxelSize, z + voxelSize + voxelSize);

			/* TODO: Calculate neighbouring voxels and remove unnessary triangles
			*/
			
			// Place the voxel
			placeCube(groundTexture, snowTexture, voxelPos, lastVoxelPos);
			lastVoxelPos = voxelPos;
		}
	}
}


//TODO: generate chunks when the player moves in that direction
void Terrain::generateChunk(Mesh& grassMesh, Mesh& mountainMesh)
{
	
}

void Terrain::placeCube(Mesh& grassMesh, Mesh& mountainMesh, glm::vec3& voxelPosition, glm::vec3& lastVoxelPosition)
{
	glm::vec3 colour(0.25, 0.25, 0.25);
	//Stores a vector of points 
	std::vector<glm::vec3> point;
	glm::vec3 null(0, 0, 0);

	//// WARNING VERY MESSY AND REPETITIVE CODE /////////
	// TODO : Optimise the code that optimses

	// Set voxels below snowPeakHeight to be ground
	if (voxelPosition.y < snowPeakHeight)
	{
		glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);

		glm::vec3 lasta(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastb(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastc(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lastd(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 laste(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastf(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lastg(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lasth(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z + voxelSize);

		// Fix rounding errors
		// Remove squares behind camera
		a.x = round(a.x); a.y = round(a.y); a.z = round(a.z);
		b.x = round(b.x); b.y = round(b.y); b.z = round(b.z);
		c.x = round(c.x); c.y = round(c.y); c.z = round(c.z);
		d.x = round(d.x); d.y = round(d.y); d.z = round(d.z);
		e.x = round(e.x); e.y = round(e.y); e.z = round(e.z);
		f.x = round(f.x); f.y = round(f.y); f.z = round(f.z);
		g.x = round(g.x); g.y = round(g.y); g.z = round(g.z);
		h.x = round(h.x); h.y = round(g.y); h.z = round(g.z);

		lasta.x = round(lasta.x); lasta.y = round(lasta.y); lasta.z = round(a.z);
		lastb.x = round(lastb.x); lastb.y = round(lastb.y); lastb.z = round(b.z);
		lastc.x = round(lastc.x); lastc.y = round(lastc.y); lastc.z = round(c.z);
		lastd.x = round(lastd.x); lastd.y = round(lastd.y); lastd.z = round(d.z);
		laste.x = round(laste.x); laste.y = round(laste.y); laste.z = round(e.z);
		lastf.x = round(lastf.x); lastf.y = round(lastf.y); lastf.z = round(f.z);
		lastg.x = round(lastg.x); lastg.y = round(lastg.y); lastg.z = round(g.z);
		lasth.x = round(lasth.x); lasth.y = round(lasth.y); lasth.z = round(g.z);
		

		if (a == lasta)
		{
			a = null;
			point.push_back(a);
		}
		else
			point.push_back(a);

		if (b == lastb)
		{
			b = null;
			point.push_back(b);
		}
		else
			point.push_back(b);

		if (c == lastc)
		{
			c = null;
			point.push_back(c);
		}
		else
			point.push_back(c);

		if (d == lastd)
		{
			d = null;
			point.push_back(d);
		}
		else
			point.push_back(d);

		if (e == laste)
		{
			e = null;
			point.push_back(e);
		}
		else
			point.push_back(e);

		if (f == lastf)
		{
			f = null;
			point.push_back(f);
		}
		else
			point.push_back(f);

		if (g == lastg)
		{
			g = null;
			point.push_back(g);
		}
		else
			point.push_back(g);

		if (h == lasth)
		{
			h = null;
			point.push_back(h);
		}
		else
			point.push_back(h);

		grassMesh.addCubeFromFace(point, colour);
	}
	// Else render snow texture cube
	else
	{

		glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);

		// Offset the mountain terrain to be lighter at the top
		glm::vec3 colour(voxelPosition.y / 50, voxelPosition.y / 50, voxelPosition.y / 50);

		glm::vec3 lasta(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastb(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastc(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lastd(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y + voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 laste(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z + voxelSize);
		glm::vec3 lastf(lastVoxelPosition.x - voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lastg(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z - voxelSize);
		glm::vec3 lasth(lastVoxelPosition.x + voxelSize, lastVoxelPosition.y - voxelSize, lastVoxelPosition.z + voxelSize);

		if (a == lasta)
		{
			a = null;
			point.push_back(a);
		}
		else
			point.push_back(a);


		if (b == lastb)
		{
			b = null;
			point.push_back(b);
		}
		else
			point.push_back(b);

		if (c == lastc)
		{
			c = null;
			point.push_back(c);
		}
		else
			point.push_back(c);

		if (d == lastd)
		{
			d = null;
			point.push_back(d);
		}
		else
			point.push_back(d);

		if (e == laste)
		{
			e = null;
			point.push_back(e);
		}
		else
			point.push_back(e);

		if (f == lastf)
		{
			f = null;
			point.push_back(f);
		}
		else
			point.push_back(f);

		if (g == lastg)
		{
			g = null;
			point.push_back(g);
		}
		else
			point.push_back(g);

		if (h == lasth)
		{
			h = null;
			point.push_back(h);
		}
		else
			point.push_back(h);

		mountainMesh.addCubeFromFace(point, colour);
	}
}

// Gets neighbouring voxels around a voxel (not used at the moment)
std::vector<Voxel> Terrain::getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels)
{
	// Vector to return
	std::vector<Voxel> result;
	float xPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().x;
	float zPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().z;

	Voxel northVoxel(xPos, voxelPosition.y, zPos + voxelSize);
	Voxel eastVoxel(xPos + voxelSize, voxelPosition.y, zPos);
	Voxel southVoxel(xPos, voxelPosition.y, zPos - voxelSize);
	Voxel westVoxel(xPos - voxelSize, voxelPosition.y, zPos);
	Voxel topVoxel(xPos, voxelPosition.y + voxelSize, zPos);
	Voxel bottomVoxel(xPos, voxelPosition.y - voxelSize, zPos);

	result.push_back(northVoxel);
	result.push_back(eastVoxel);
	result.push_back(southVoxel);
	result.push_back(westVoxel);
	result.push_back(topVoxel);
	result.push_back(bottomVoxel);
	return result;
}