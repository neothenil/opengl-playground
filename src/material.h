#pragma once

#include <string>
#include <cstring>
#include <map>
#include <memory>

class MaterialProperty {
public:
    enum Type { Unknown, Uniform1i, Uniform1f, Uniform3fv, UniformMatrix4fv };

    std::unique_ptr<void, void(*)(void*)> mData;
    unsigned int mSize;
    Type mType;

public:
    MaterialProperty() 
        : mData(nullptr, [](void*){}), mSize(0), mType(Unknown) {}

    MaterialProperty(const void* ptr, unsigned int size, Type type)
        : mSize(size), mType(type), mData(operator new(size), [](void* p) { operator delete(p); }) {
        std::memcpy(mData.get(), ptr, size);
    }

    template<typename T>
    MaterialProperty(const T* ptr, Type type)
        : MaterialProperty(ptr, sizeof(T), type) {}

    MaterialProperty(const MaterialProperty& other)
        : mSize(other.mSize), mType(other.mType), mData(operator new(other.mSize), [](void* p) { operator delete(p); }) {
        std::memcpy(mData.get(), other.mData.get(), other.mSize);
    }

    MaterialProperty& operator=(const MaterialProperty& other) {
        if (this != &other) {
            mSize = other.mSize;
            mType = other.mType;
            mData.reset(operator new(other.mSize));
            mData.get_deleter() = [](void* p){ operator delete(p); };
            std::memcpy(mData.get(), other.mData.get(), other.mSize);
        }
        return *this;
    }

    ~MaterialProperty() = default;

    void setType(Type type) { mType = type; }
};

class MaterialTexture
{
    int mWidth;
    int mHeight;
    int mChannels;
    unsigned int mTextureId = 0;
    std::unique_ptr<unsigned char, void(*)(void*)> mData;

    static bool stbInitialized;

public:
    MaterialTexture(): mWidth(0), mHeight(0), mChannels(0), mData(nullptr, [](void*){}) {}
    explicit MaterialTexture(const std::string& path);
    MaterialTexture(const MaterialTexture& other);
    MaterialTexture& operator=(const MaterialTexture& other);
    ~MaterialTexture();
    unsigned int textureId();
};

class Material
{
    // map from variable name in shader code to its data
    std::map<std::string, MaterialProperty> mUniforms;
    std::map<std::string, MaterialTexture> mTextures;

public:
    Material() = default;
    Material(const std::map<std::string, MaterialProperty>& uniforms,
        const std::map<std::string, MaterialTexture>& textures)
        : mUniforms(uniforms), mTextures(textures) {}
    void addUniform(const std::string& name, const MaterialProperty& property);
    void addTexture(const std::string& name, const MaterialTexture& texture);
    const MaterialProperty* getUniform(const std::string& name) const;
    MaterialTexture* getTexture(const std::string& name);
};
