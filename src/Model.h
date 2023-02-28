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
	void RenderModel() const;
	void ClearModel();
	~Model();

private:
	void LoadNode(const aiNode* node, const aiScene* scene);
	void LoadMesh(const aiMesh* mesh, const aiScene* scene);
	void LoadMaterial(const aiScene* scene);

	vector<Mesh*> meshList;
	vector<Texture*> textureList;
	vector<unsigned int> meshToTex;
};

