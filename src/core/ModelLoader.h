#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <string>
#include <vector>
#include <memory>

class ModelLoader
{
public:
	std::vector<std::shared_ptr<Mesh>> LoadModel(const std::string& path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
