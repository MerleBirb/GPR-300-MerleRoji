//Author: Eric Winebrenner

#pragma once
#include "Mesh.h"

namespace ew {
	void createPlane(float width, float height, MeshData& meshData);
	void createQuad(float width, float height, MeshData& meshData);
	void createCube(float width, float height, float depth, MeshData& meshData);
	void createSphere(float radius, int numSegments, MeshData& meshData);
	void createCylinder(float height, float radius, int numSegments, MeshData& meshData);
}
