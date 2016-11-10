#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (m_positionBuffer != 0)
		glDeleteBuffers(1, &m_positionBuffer);

	if (m_colourBuffer != 0)
		glDeleteBuffers(1, &m_colourBuffer);

	if (m_uvBuffer != 0)
		glDeleteBuffers(1, &m_uvBuffer);

	if (m_normalBuffer != 0)
		glDeleteBuffers(1, &m_normalBuffer);
}

void Mesh::addVertex(const Vertex& vertex)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add vertices after createBuffers() has been called");
	}

	m_vertexPositions.push_back(vertex.m_position);
	m_vertexColours.push_back(vertex.m_colour);
	m_vertexUVs.push_back(vertex.m_textureCoord);
	m_vertexNormals.push_back(vertex.m_normal);
}

void Mesh::addTriangleVertex(Vertex v1, Vertex v2, Vertex v3)
{
	glm::vec3 normal = glm::cross(v2.m_position - v1.m_position, v3.m_position - v1.m_position);
	normal = glm::normalize(normal);

	v1.m_normal = normal;
	v2.m_normal = normal;
	v3.m_normal = normal;

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
}

void Mesh::addSquareVertex(Vertex v1, Vertex v2, Vertex v3, Vertex v4)
{
	addTriangleVertex(v1, v2, v4);
	addTriangleVertex(v1, v3, v4);
}

void Mesh::addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
	const glm::vec3& colour,
	const glm::vec2& t1, const glm::vec2& t2, const glm::vec2& t3)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add triangles after createBuffers() has been called");
	}

	// Add the vertex positions
	m_vertexPositions.push_back(p1);
	m_vertexPositions.push_back(p2);
	m_vertexPositions.push_back(p3);
	
	// Make all three vertices the same colour
	for (int i = 0; i < 3; i++)
		m_vertexColours.push_back(colour);

	// Add the texture coordinates
	m_vertexUVs.push_back(t1);
	m_vertexUVs.push_back(t2);
	m_vertexUVs.push_back(t3);


	glm::vec3 edge1(p3 - p1);
	glm::vec3 edge2(p2 - p1);

	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

	m_vertexNormals.push_back(normal);

}


void Mesh::addSquare(const glm::vec3& a, const glm::vec3& b,
	const glm::vec3& c, const glm::vec3& d, const glm::vec3& colour,
	float u1, float u2, float v1, float v2)
{
	glm::vec2 ta(u1, v1);
	glm::vec2 tb(u1, v2);
	glm::vec2 tc(u2, v2);
	glm::vec2 td(u2, v1);

	addTriangle(a, b, d, colour, ta, tb, td);
	addTriangle(d, b, c, colour, td, tb, tc);
}

void Mesh::addCube(const glm::vec3& a, const glm::vec3& b,const glm::vec3& c,const glm::vec3& d, const glm::vec3& e, const glm::vec3& f, const glm::vec3& g, const glm::vec3& h, const glm::vec3& colour)
{
	addSquare(a, b, c, d, colour, 0.0f, 0.0f, 0.0f, 0.0f);
	addSquare(b, h, g, c, colour, 0.0f, 0.0f, 0.0f, 0.0f);
	addSquare(a, e, h, b, colour, 0.0f, 0.0f, 0.0f, 0.0f);
	addSquare(d, f, e, a, colour, 0.0f, 0.0f, 0.0f, 0.0f);
	addSquare(e, f, g, h, colour, 0.0f, 0.0f, 0.0f, 0.0f);
	addSquare(d, c, g, f, colour, 0.0f, 0.0f, 0.0f, 0.0f);
}

void Mesh::addCircle(const glm::vec3& centre, float radius, int numPoints,
	const glm::vec3& colour)
{
	float angleStep = glm::radians(360.0f) / numPoints;

	glm::vec3 lastEdgePoint = centre + radius * glm::vec3(1, 0, 0);

	for (int i = 1; i <= numPoints; i++)
	{
		float angle = angleStep * i;
		glm::vec3 nextEdgePoint = centre + radius * glm::vec3(cos(angle), 0, sin(angle));
		addTriangle(centre, lastEdgePoint, nextEdgePoint, colour, glm::vec2(), glm::vec2(), glm::vec2());
		lastEdgePoint = nextEdgePoint;
	}
}

void Mesh::addCylinder(const glm::vec3& centre, float radius, int numPoints, float height,
	const glm::vec3& colour)
{
	float angleStep = glm::radians(360.0f) / numPoints;

	glm::vec3 lastEdgePoint = centre + radius * glm::vec3(1, 0, 0);

	for (int i = 1; i <= numPoints; i++)
	{
		float angle = angleStep * i;
		glm::vec3 nextEdgePoint = centre + radius * glm::vec3(cos(angle), 0, sin(angle));
		addTriangle(centre, lastEdgePoint, nextEdgePoint, colour, glm::vec2(), glm::vec2(), glm::vec2());
		addSquare(lastEdgePoint, nextEdgePoint, nextEdgePoint + glm::vec3(0, height, 0), lastEdgePoint + glm::vec3(0, height, 0), colour, 0.0f, 0.0f, 0.0f, 0.0f);
		
		lastEdgePoint = nextEdgePoint;
	}
	addCircle(glm::vec3(centre.x, centre.y + height, centre.z), radius, numPoints, colour);

}

void Mesh::createBuffers()
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("createBuffers() has already been called");
	}

	// Create and fill the position buffer
	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexPositions.size() * sizeof(glm::vec3), m_vertexPositions.data(), GL_STATIC_DRAW);

	// Create and fill the colour buffer
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexColours.size() * sizeof(glm::vec3), m_vertexColours.data(), GL_STATIC_DRAW);

	// Create and fill the texture coordinate buffer
	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexUVs.size() * sizeof(glm::vec2), m_vertexUVs.data(), GL_STATIC_DRAW);

	// Create and fill the normal buffer
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * sizeof(glm::vec3), m_vertexNormals.data(), GL_STATIC_DRAW);
}

void Mesh::draw()
{
	if (m_positionBuffer == 0)
	{
		throw std::exception("createBuffers() must be called before draw()");
	}

	// Bind the position buffer to vertex attribute 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the colour buffer to vertex attribute 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the texture coordinate buffer to vertex attribute 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the normal buffer to vertex attribute 3
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
