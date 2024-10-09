#include "shader.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>

std::string readFile(const std::string& path)
{
    std::ifstream fileStream;
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fileStream.open(path);
    std::stringstream strStream;
    strStream << fileStream.rdbuf();
    fileStream.close();
    return strStream.str();
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource,
      const std::vector<std::string>& uniforms, const std::vector<std::string>& textures)
    : mUniforms(uniforms), mTextures(textures)
{
    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();
    unsigned int vertexId = 0, fragmentId = 0;
    int success = 0;
    char glLogBuffer[512];
    // vertex shader
    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertexCode, NULL);
    glCompileShader(vertexId);
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, 512, NULL, glLogBuffer);
        std::string msg = "Failed to compile vertex shader: ";
        msg += glLogBuffer;
        throw OpenGLException(msg);
    }
    // fragment shader
    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragmentCode, NULL);
    glCompileShader(fragmentId);
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, 512, NULL, glLogBuffer);
        std::string msg = "Failed to compile fragment shader: ";
        msg += glLogBuffer;
        throw OpenGLException(msg);
    }
    // program
    mId = glCreateProgram();
    glAttachShader(mId, vertexId);
    glAttachShader(mId, fragmentId);
    glLinkProgram(mId);
    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mId, 512, NULL, glLogBuffer);
        std::string msg = "Failed to link shader program: ";
        msg += glLogBuffer;
        throw OpenGLException(msg);
    }
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
    // find uniform and texture locations
    for (const auto& variable : mUniforms) {
        int location = glGetUniformLocation(mId, variable.c_str());
        mUniformLocations[variable] = location;
    }
    for (const auto& variable : mTextures) {
        int location = glGetUniformLocation(mId, variable.c_str());
        mUniformLocations[variable] = location;
    }
}

Shader::Shader(Shader&& other)
  : mId(other.mId), mUniformLocations(std::move(other.mUniformLocations)),
  mUniforms(std::move(other.mUniforms)), mTextures(std::move(other.mTextures))
{
    other.mId = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    if (this != &other) {
        if (mId != 0) glDeleteProgram(mId);
        mId = other.mId;
        other.mId = 0;
        mUniformLocations = std::move(other.mUniformLocations);
        mUniforms = std::move(other.mUniforms);
        mTextures = std::move(other.mTextures);
    }
    return *this;
}

Shader::~Shader()
{
    if (mId != 0) {
        glDeleteProgram(mId);
    }
}

void Shader::use()
{
    if (mId != 0 && glIsProgram(mId)) {
        glUseProgram(mId);
    }
    else {
        throw OpenGLException("Failed to use shader program.");
    }
}

void Shader::setFromMaterial(Material* mat)
{
    for (const auto& uName: mUniforms) {
        const MaterialProperty* prop = mat->getUniform(uName);
        if (prop == nullptr) continue;
        setMaterialProperty(uName, prop);
    }
    int textureUnit = 0;
    for (const auto& texName: mTextures) {
        MaterialTexture* texture = mat->getTexture(texName);
        if (texture == nullptr) {
            ++textureUnit;
            continue;
        }
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
        setInt(texName, textureUnit);
    }
}

void Shader::setMaterialProperty(const std::string& name, const MaterialProperty* prop)
{
    switch (prop->mType) {
    case MaterialProperty::Uniform1i:
        glUniform1i(mUniformLocations.at(name), *(int*)(prop->mData.get()));
        break;
    case MaterialProperty::Uniform1f:
        glUniform1f(mUniformLocations.at(name), *(float*)(prop->mData.get()));
        break;
    case MaterialProperty::Uniform3fv:
        glUniform3fv(mUniformLocations.at(name), 1, (float*)(prop->mData.get()));
        break;
    case MaterialProperty::UniformMatrix4fv:
        glUniformMatrix4fv(mUniformLocations.at(name), 1, GL_FALSE, (float*)(prop->mData.get()));
        break;
    }
}
