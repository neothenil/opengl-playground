
#include "material.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include <glad/glad.h>

bool MaterialTexture::stbInitialized = false;

MaterialTexture::MaterialTexture(const std::string& path)
: mWidth(0), mHeight(0), mChannels(0), mData(nullptr, stbi_image_free)
{
    if (!stbInitialized) {
        stbi_set_flip_vertically_on_load(true);
        stbInitialized = true;
    }
    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
    mData.reset(data);
}

MaterialTexture::MaterialTexture(const MaterialTexture& other)
: mWidth(other.mWidth), mHeight(other.mHeight), mChannels(other.mChannels),
  mData(nullptr, [](void* p){ operator delete(p); })
{
    unsigned int size = mWidth * mHeight * mChannels;
    unsigned char* data = static_cast<unsigned char*>(operator new(size));
    std::memcpy(data, other.mData.get(), size);
    mData.reset(data);
}

MaterialTexture& MaterialTexture::operator=(const MaterialTexture& other)
{
    if (this != &other) {
        mWidth = other.mWidth;
        mHeight = other.mHeight;
        mChannels = other.mChannels;
        unsigned int size = mWidth * mHeight * mChannels;
        unsigned char* data = static_cast<unsigned char*>(operator new(size));
        std::memcpy(data, other.mData.get(), size);
        mData.reset(data);
        mData.get_deleter() = [](void* p){ operator delete(p); };
        // Only copy source data. Texture object on GPU is generated on demand.
        if (mTextureId != 0) {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
        }
    }
    return *this;
}

MaterialTexture::~MaterialTexture()
{
    if (mTextureId != 0) {
        glDeleteTextures(1, &mTextureId);
        mTextureId = 0;
    }
}

unsigned int MaterialTexture::textureId()
{
    if (mTextureId != 0)
        return mTextureId;
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return mTextureId;
}

void Material::addUniform(const std::string& name, const MaterialProperty& property)
{
    mUniforms[name] = property;
}

void Material::addTexture(const std::string& name, const MaterialTexture& texture)
{
    mTextures[name] = texture;
}

const MaterialProperty* Material::getUniform(const std::string& name) const
{
    try {
        const MaterialProperty& prop = mUniforms.at(name);
        return &prop;
    }
    catch (const std::out_of_range&) {
        return nullptr;
    }
}

MaterialTexture* Material::getTexture(const std::string& name)
{
    try {
        MaterialTexture& texture = mTextures.at(name);
        return &texture;
    }
    catch (const std::out_of_range&) {
        return nullptr;
    }
} 
