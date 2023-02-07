#pragma once

#include "GL/glew.h"

class Mesh
{
public:
	Mesh();
	void CreateMesh(const float* vertices, const unsigned int* indices, unsigned int numOfVertices, int numOfIndices);
	void RenderMesh() const;
	void ClearMesh();
	~Mesh();

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	int indexCount;

};

