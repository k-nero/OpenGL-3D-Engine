#include "Model.h"

#include <iostream>

using namespace Assimp;

Model::Model()
= default;

Model::Model(const Model& other)
{
	meshList = other.meshList;
	meshToTex = other.meshToTex;
	textureList = other.textureList;
}

Model& Model::operator=(const Model& other)
{
	if(this != &other)
	{
		meshList = other.meshList;
		meshToTex = other.meshToTex;
		textureList = other.textureList;
	}
	return * this;
}

// Move constructor
Model::Model(Model&& other) noexcept
{
	meshList = std::move(other.meshList);
	meshToTex = std::move(other.meshToTex);
	textureList = std::move(other.textureList);
}

// Move assignment operator
Model& Model::operator=(Model&& other) noexcept
{
	if(this != &other)
	{
		meshList = std::move(other.meshList);
		meshToTex = std::move(other.meshToTex);
		textureList = std::move(other.textureList);
	}
	return * this;
}

void Model::LoadModel(const string& fileName)
{
	Importer importer;
	const aiScene * scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		cout << "Model "<< fileName <<" failed to load : " << importer.GetErrorString() << endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterial(scene);
}

void Model::LoadNode(const aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(const aiMesh* mesh, const aiScene* scene)
{
	vector<float> vertices;
	vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto * newMesh = new Mesh();
	newMesh->CreateMesh(vertices.data(), indices.data(), static_cast<int>(vertices.size()), static_cast<int>(indices.size()));
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterial(const aiScene * scene)
{
	textureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				const int idx = string(path.data).rfind('\\');
				string filename = string(path.data).substr(idx + 1);
				string texPath = string("textures/") + filename;
				textureList[i] = new Texture(texPath.c_str(), true);
				if (!textureList[i]->LoadTexture())
				{
					cout << "Failed to load texture at: " << texPath << endl;
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		if(material->GetTextureCount(aiTextureType_SPECULAR))
		{
			cout << "Specular texture found!!!"  << endl;
		}
		if (material->GetTextureCount(aiTextureType_NORMALS))
		{
			cout << "Normal texture found!!!" << endl;
		}
		if (material->GetTextureCount(aiTextureType_OPACITY))
		{
			cout << "Alpha texture found!!!" << endl;
		}

		if (!textureList[i])
		{
			textureList[i] = new Texture("textures/diamond.png", true);
			textureList[i]->LoadTextureAlpha();
		}
	}
}

void Model::RenderModel() const
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		const unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (auto& i : meshList)
	{
		if (i)
		{
			i->ClearMesh();
			delete i;
			i = nullptr;
		}
	}

	for (auto& i : textureList)
	{
		if (i)
		{
			i->ClearTexture();
			delete i;
			i = nullptr;
		}
	}
}

Model::~Model()
{
	ClearModel();
}


