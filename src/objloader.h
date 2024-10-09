#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "renderer.h"

class ObjLoader
{
    static Material* processMaterial(const aiMaterial* aiMat, const std::string& dirPath);
    static void processNode(Renderer& renderer, const aiNode* node, const aiScene* scene);
    static Mesh* processMesh(Renderer& renderer, const aiMesh* mesh, const aiScene* scene);
public:
    static void loadObjFile(Renderer& renderer, const std::string& path);
};
