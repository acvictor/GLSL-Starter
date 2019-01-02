#pragma once

#include <bits/stdc++.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model 
{
public:
	Model();

	void LoadModel(const std::string& filename);
	void RenderModel();
	void ClearModel();

	~Model();

private:

	// Assimp variables, don't want scene to change
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;

	// Some meshes might use same texture
	std::vector<unsigned int> meshToTex;
};