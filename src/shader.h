#pragma once

#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "material.h"

std::string readFile(const std::string& path);

class Shader
{
public: // for debug
    unsigned int mId = 0;  // OpenGL program id
    // Uniform location for all uniforms (including textures)
    std::map<std::string, int> mUniformLocations;
    // Uniform names except textures
    std::vector<std::string> mUniforms;
    // Texture names
    std::vector<std::string> mTextures;

public:
    Shader() = default;
    /** Constructor reads and builds the shader */
    Shader(const std::string& vertexSource, const std::string& fragmentSource,
      const std::vector<std::string>& uniforms = std::vector<std::string>(),
      const std::vector<std::string>& textures = std::vector<std::string>());
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);
    virtual ~Shader();
    /** Use/activate the shader */
    void use();
    /** Utility uniform functions */
    void setBool(const std::string& name, bool value) const
    { glUniform1i(mUniformLocations.at(name), (int)value); }
    void setInt(const std::string& name, int value) const
    { glUniform1i(mUniformLocations.at(name), value); }
    void setFloat(const std::string& name, float value) const
    { glUniform1f(mUniformLocations.at(name), value); } 
    void setVec2(const std::string& name, const glm::vec2& value) const
    { glUniform2fv(mUniformLocations.at(name), 1, &value[0]); }
    void setVec2(const std::string& name, float x, float y) const
    { glUniform2f(mUniformLocations.at(name), x, y); }
    void setVec3(const std::string& name, const glm::vec3& value) const
    { glUniform3fv(mUniformLocations.at(name), 1, &value[0]); }
    void setVec3(const std::string& name, float x, float y, float z) const
    { glUniform3f(mUniformLocations.at(name), x, y, z); }
    void setVec4(const std::string& name, const glm::vec4& value) const
    { glUniform4fv(mUniformLocations.at(name), 1, &value[0]); }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    { glUniform4f(mUniformLocations.at(name), x, y, z, w); }
    void setMat2(const std::string& name, const glm::mat2& mat) const
    { glUniformMatrix2fv(mUniformLocations.at(name), 1, GL_FALSE, &mat[0][0]); }
    void setMat3(const std::string& name, const glm::mat3& mat) const
    { glUniformMatrix3fv(mUniformLocations.at(name), 1, GL_FALSE, &mat[0][0]); }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    { glUniformMatrix4fv(mUniformLocations.at(name), 1, GL_FALSE, &mat[0][0]); }
    void setFromMaterial(Material* mat);
    void setMaterialProperty(const std::string& name, const MaterialProperty* prop);
};
