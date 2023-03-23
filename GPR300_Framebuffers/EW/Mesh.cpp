//Author: Eric Winebrenner

#include "Mesh.h"
namespace ew {
	Mesh::Mesh(MeshData* meshData) {

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, meshData->vertices.size() * sizeof(Vertex), &meshData->vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indices.size() * sizeof(unsigned int), &meshData->indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		mNumIndices = (GLsizei)meshData->indices.size();
		mNumVertices = (GLsizei)meshData->vertices.size();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}

	void Mesh::draw()
	{
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
	}

}
