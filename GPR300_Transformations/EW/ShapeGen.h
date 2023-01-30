#pragma once
#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>

void createQuad(float width, float height, MeshData& meshData);
void createCube(float width, float height, float depth, MeshData& meshData);
void createSphere(float radius, int numSegments, MeshData& meshData);

void createQuad(float width, float height, MeshData& meshData) {
	meshData.vertices.clear();
	meshData.indices.clear();
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	Vertex vertices[4] = {
		//Front face
		{glm::vec3(-halfWidth, -halfHeight, 0), glm::vec3(0,0,1)}, //BL
		{glm::vec3(+halfWidth, -halfHeight, 0), glm::vec3(0,0,1)}, //BR
		{glm::vec3(+halfWidth, +halfHeight, 0), glm::vec3(0,0,1)}, //TR
		{glm::vec3(-halfWidth, +halfHeight, 0), glm::vec3(0,0,1)} //TL
	};
	meshData.vertices.assign(&vertices[0], &vertices[4]);
	unsigned int indices[6] = {
		// front face
		0, 1, 2,
		0, 2, 3
	};
	meshData.indices.assign(&indices[0], &indices[6]);
};

void createCube(float width, float height, float depth, MeshData& meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	//VERTICES
	//-------------
	Vertex vertices[24] = {
		//Front face
		{glm::vec3(-halfWidth, -halfHeight, +halfDepth), glm::vec3(0,0,1)}, //BL
		{glm::vec3(+halfWidth, -halfHeight, +halfDepth), glm::vec3(0,0,1)}, //BR
		{glm::vec3(+halfWidth, +halfHeight, +halfDepth), glm::vec3(0,0,1)}, //TR
		{glm::vec3(-halfWidth, +halfHeight, +halfDepth), glm::vec3(0,0,1)}, //TL

		//Back face
		{glm::vec3(+halfWidth, -halfHeight, -halfDepth), glm::vec3(0,0,-1)}, //BL
		{glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(0,0,-1)}, //BR
		{glm::vec3(-halfWidth, +halfHeight, -halfDepth), glm::vec3(0,0,-1)}, //TR
		{glm::vec3(+halfWidth, +halfHeight, -halfDepth), glm::vec3(0,0,-1)}, //TL

		//Right face
		{glm::vec3(+halfWidth, -halfHeight, +halfDepth), glm::vec3(1,0,0)}, //BL
		{glm::vec3(+halfWidth, -halfHeight, -halfDepth), glm::vec3(1,0,0)}, //BR
		{glm::vec3(+halfWidth, +halfHeight, -halfDepth), glm::vec3(1,0,0)}, //TR
		{glm::vec3(+halfWidth, +halfHeight, +halfDepth), glm::vec3(1,0,0)}, //TL

		//Left face
		{glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(-1,0,0)}, //BL
		{glm::vec3(-halfWidth, -halfHeight, +halfDepth), glm::vec3(-1,0,0)}, //BR
		{glm::vec3(-halfWidth, +halfHeight, +halfDepth), glm::vec3(-1,0,0)}, //TR
		{glm::vec3(-halfWidth, +halfHeight, -halfDepth), glm::vec3(-1,0,0)}, //TL

		//Top face
		{glm::vec3(-halfWidth, +halfHeight, +halfDepth), glm::vec3(0,1,0)}, //BL
		{glm::vec3(+halfWidth, +halfHeight, +halfDepth), glm::vec3(0,1,0)}, //BR
		{glm::vec3(+halfWidth, +halfHeight, -halfDepth), glm::vec3(0,1,0)}, //TR
		{glm::vec3(-halfWidth, +halfHeight, -halfDepth), glm::vec3(0,1,0)}, //TL

		//Bottom face
		{glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(0,-1,0)}, //BL
		{glm::vec3(+halfWidth, -halfHeight, -halfDepth), glm::vec3(0,-1,0)}, //BR
		{glm::vec3(+halfWidth, -halfHeight, +halfDepth), glm::vec3(0,-1,0)}, //TR
		{glm::vec3(-halfWidth, -halfHeight, +halfDepth), glm::vec3(0,-1,0)}, //TL
	};
	meshData.vertices.assign(&vertices[0], &vertices[24]);

	//INDICES
	//-------------

	unsigned int indices[36] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 5, 6,
		6, 7, 4,

		// right face
		8,  9, 10,
		10, 11, 8,

		//left face 
		12, 13, 14,
		14, 15, 12,

		//top face
		16,17,18,
		18,19,16,

		//bottom face
		20, 21, 22,
		22, 23, 20
	};
	meshData.indices.assign(&indices[0], &indices[36]);
}

void createSphere(float radius, int numSegments, MeshData& meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	float topY = radius;
	float bottomY = -radius;

	unsigned int topIndex = 0;
	meshData.vertices.push_back({ glm::vec3(0,topY,0),glm::vec3(0,1,0)});

	//Angle between segments
	float thetaStep = (2.0f * glm::pi<float>()) / (float)numSegments;
	float phiStep = (glm::pi<float>()) / (float)numSegments;

	for (int i = 1; i < numSegments; i++)
	{
		float phi = phiStep * i;

		//Create row
		for (int j = 0; j <= numSegments; ++j)
		{
			float theta = thetaStep * j;

			float x = radius * sinf(phi) * sinf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * cosf(theta);

			glm::vec3 position = glm::vec3(x, y, z);
			glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

			meshData.vertices.push_back({ position, normal });
		}
	}

	meshData.vertices.push_back({ glm::vec3(0,bottomY,0), glm::vec3(0,-1,0)});
	unsigned int bottomIndex = (unsigned int)meshData.vertices.size() - 1;
	unsigned int ringVertexCount = numSegments + 1;

	//TOP CAP
	for (int i = 0; i < numSegments; ++i) {
		meshData.indices.push_back(topIndex); //top cap center 
		meshData.indices.push_back(i + 1);
		meshData.indices.push_back(i + 2);
	}

	//RINGS
	unsigned int start = 1;

	//Row index
	//-2 to ignore poles
	for (int y = 0; y < numSegments - 2; ++y)
	{
		//Column index
		for (int x = 0; x < numSegments; ++x)
		{
			//Triangle 1
			meshData.indices.push_back(start + y * ringVertexCount + x);
			meshData.indices.push_back(start + (y + 1) * ringVertexCount + x);
			meshData.indices.push_back(start + y * ringVertexCount + x + 1);

			//Triangle 2
			meshData.indices.push_back(start + y * ringVertexCount + x + 1);
			meshData.indices.push_back(start + (y + 1) * ringVertexCount + x);
			meshData.indices.push_back(start + (y + 1) * ringVertexCount + x + 1);
		}
	}

	start = bottomIndex - ringVertexCount;

	//BOTTOM CAP
	for (unsigned int i = 0; i < ringVertexCount; ++i) {
		meshData.indices.push_back(start + i + 1);
		meshData.indices.push_back(start + i);
		meshData.indices.push_back(bottomIndex); //bottom cap center 
	}
}