#include "Mesh.h"
#include "GL/glew.h"
#include <cstddef>
#include <string>
#include <utility>

#include "Shader.h"

using namespace std;

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::SetTextures(vector<Texture> textureList)
{
	textures = std::move(textureList);
}

void Mesh::CreateMesh(const float* vertices, const unsigned int* indices, int numOfVertices, int numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long long>(sizeof(indices[0]) * numOfIndices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(sizeof(vertices[0]) * numOfVertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, reinterpret_cast<void*>(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, reinterpret_cast<void*>(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}

void Mesh::RenderMesh(const Shader & shader) const
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].GetTextureType();
		if (name == "texture_diffuse")
		{
			number = to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = to_string(specularNr++);
		}
		else if (name == "texture_norma")
		{
			number = to_string(normalNr++);
		}
		else if (name == "texture_height")
		{
			number = to_string(heightNr++);
		}
		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.getShaderIdI(), (name + number).c_str()), static_cast<int>(i));
		glBindTexture(GL_TEXTURE_2D, textures[i].GetTextureID());
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::ClearMesh()
{
	if(IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if(VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}
