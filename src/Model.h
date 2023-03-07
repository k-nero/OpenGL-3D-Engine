#pragma once

#include "vector"
#include "string"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Texture.h"

using namespace std;

class Model
{
public:
	Model();
	Model(const Model& other);
	Model& operator=(const Model& other);
	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;
	void LoadModel(const string& fileName);
	void RenderModel(const Shader& shader) const;
	void ClearModel();
	~Model() { ClearModel(); } 

private:
	void LoadNode(const aiNode* node, const aiScene* scene);
	Mesh * LoadMesh(const aiMesh* mesh, const aiScene* scene);
	vector<Texture*> LoadMaterial(const aiMaterial* material, aiTextureType type, const string& typeName);

	vector<Mesh*> meshList;
	vector<Texture> textures_loaded;
	vector<unsigned int> meshToTex;
};

