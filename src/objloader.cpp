#include "objloader.h"
#include <filesystem>

void ObjLoader::loadObjFile(Renderer& renderer, const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    std::filesystem::path fsPath = path;
    std::string dirPath = fsPath.parent_path().string();
    if (scene == nullptr)
        return;
    // Load materials
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* aiMat = scene->mMaterials[i];
        renderer.addMaterial(processMaterial(aiMat, dirPath));
    }
    // Load meshes
    processNode(renderer, scene->mRootNode, scene);
}

Material* ObjLoader::processMaterial(const aiMaterial* aiMat, const std::string& dirPath)
{
    Material* mat = new Material();
    aiColor3D diffuse, ambient, specular;
    aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mat->addUniform("uKd", MaterialProperty(&diffuse, MaterialProperty::Uniform3fv));
    mat->addUniform("uKs", MaterialProperty(&specular, MaterialProperty::Uniform3fv));
    mat->addUniform("uKa", MaterialProperty(&ambient, MaterialProperty::Uniform3fv));
    int textureSample = 0;
    if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        std::filesystem::path fsPath = dirPath;
        aiString texturePath;
        aiMat->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), texturePath);
        std::filesystem::path fsTexturePath = texturePath.C_Str();
        if (fsTexturePath.is_absolute()) {
            fsPath = fsTexturePath;
        } else {
            fsPath /= fsTexturePath;
        }
        MaterialTexture texture(fsPath.string());
        mat->addTexture("uSampler", texture);
        textureSample = 1;
    }
    mat->addUniform("uTextureSample", MaterialProperty(&textureSample, MaterialProperty::Uniform1i));
    return mat;
}

void ObjLoader::processNode(Renderer& renderer, const aiNode* node, const aiScene* scene)
{
    const aiMatrix4x4& t = node->mTransformation;
    glm::mat4 transform(glm::vec4(t.a1, t.b1, t.c1, t.d1),
                        glm::vec4(t.a2, t.b2, t.c2, t.d2),
                        glm::vec4(t.a3, t.b3, t.c3, t.d3),
                        glm::vec4(t.a4, t.b4, t.c4, t.d4));
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* m = processMesh(renderer, mesh, scene);
        m->setTransform(transform);
        renderer.addMesh(m);
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(renderer, node->mChildren[i], scene);
    }
}

Mesh* ObjLoader::processMesh(Renderer& renderer, const aiMesh* mesh, const aiScene* scene)
{
    Mesh* m = new Mesh();
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> faceIndices;
    glm::mat4 transform(1.0f);
    Material* mat = nullptr;
    // vertices, normals and texCoords
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertices.push_back(position);
        normals.push_back(normal);
        if (mesh->mTextureCoords[0]) {
            glm::vec2 texCoord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            texCoords.push_back(texCoord);
        } else {
            texCoords.push_back(glm::vec2(0.0, 0.0));
        }
    }
    // faceIndices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            faceIndices.push_back(face.mIndices[j]);
        }
    }
    // material
    if (mesh->mMaterialIndex >= 0) {
        mat = renderer.mMaterials[mesh->mMaterialIndex].get();
    }
    m->setVertices(vertices);
    m->setNormals(normals);
    m->setTexCoords(texCoords);
    m->setFaceIndices(faceIndices);
    m->setTransform(transform);
    m->setMaterial(mat);
    m->setup();
    return m;
}
