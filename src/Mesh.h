#pragma once
#include <vector>

#include "Shader.h"
#include "Texture.h"

using namespace std;

class Mesh
{
public:
	Mesh();
	void CreateMesh(const float* vertices, const unsigned int* indices, int numOfVertices, int numOfIndices);
	void RenderMesh(const Shader& shader) const;
	void SetTextures(vector<Texture> textureList);
	void ClearMesh();
	~Mesh() { ClearMesh(); }

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	int indexCount;

	vector<Texture> textures;
};

