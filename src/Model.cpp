#include "Model.h"

#include <iostream>

using namespace Assimp;

Model::Model()
= default;

Model::Model(const Model& other)
{
	meshList = other.meshList;
	textures_loaded = other.textures_loaded;
}

Model& Model::operator=(const Model& other)
{
	if(this != &other)
	{
		meshList = other.meshList;
		textures_loaded = other.textures_loaded;
	}
	return * this;
}

// Move constructor
Model::Model(Model&& other) noexcept
{
	meshList = std::move(other.meshList);
	textures_loaded = std::move(other.textures_loaded);
}

// Move assignment operator
Model& Model::operator=(Model&& other) noexcept
{
	if(this != &other)
	{
		meshList = std::move(other.meshList);
		textures_loaded = std::move(other.textures_loaded);
	}
	return * this;
}

void Model::LoadModel(const string& fileName)
{
	Importer importer;
	const aiScene * scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "Model "<< fileName <<" failed to load : " << importer.GetErrorString() << endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	if(!textures_loaded.empty())
	{
		cout << "Model " << fileName << " loaded successfully, destroying temp" << endl;
		textures_loaded.clear();
	}
}

void Model::LoadNode(const aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshList.push_back(LoadMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

Mesh * Model::LoadMesh(const aiMesh* mesh, const aiScene* scene)
{
	vector<float> vertices;
	vector<unsigned int> indices;
	vector<Texture*> textures;

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


	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shader. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture*> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture*> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	vector<Texture*> normalMaps = LoadMaterial(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	vector<Texture*> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	const auto newMesh = new Mesh();
	newMesh->CreateMesh(vertices.data(), indices.data(), static_cast<int>(vertices.size()), static_cast<int>(indices.size()));
	newMesh->SetTextures(textures);
	return newMesh;
}

vector<Texture*> Model::LoadMaterial(const aiMaterial * material, const aiTextureType type, const string& typeName)
{
	vector<Texture*> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		auto pathStr = (string("textures/") + string(str.C_Str()));
		auto * pathData = new char[pathStr.length() + 1];
		strcpy_s(pathData, pathStr.length() + 1, pathStr.c_str());
		for (auto& j : textures_loaded)
		{
			if (strcmp(j->GetFileLocation(), pathData) == 0)
			{
				textures.push_back(j);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			auto texture = new Texture(pathData, true);
			texture->SetTextureType(typeName);
			if( texture->LoadTexture())
			{
				cout << "Texture " << pathData << " loaded successfully" << endl;
			}
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;
}

void Model::RenderModel(const Shader& shader) const
{
	for (auto & mesh : meshList)
	{
		mesh->RenderMesh(shader);
	}
}

void Model::ClearModel()
{
	cout << "Destroying model " << endl;
	for (auto& mesh : meshList)
	{
		if (mesh)
		{
			mesh->~Mesh();
			delete mesh;
			mesh = nullptr;
		}
	}
	for(auto & tmpTextures : textures_loaded)
	{
		if(tmpTextures)
		{
			tmpTextures->~Texture();
			delete tmpTextures;
			tmpTextures = nullptr;
		}
	}
}