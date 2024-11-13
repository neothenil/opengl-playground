#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include "material.h"
#include "camera.h"

class Renderer
{
    friend class ObjLoader;

    std::vector<std::unique_ptr<Mesh>> mMeshes;
    std::vector<std::unique_ptr<Material>> mMaterials;
    int mWidth, mHeight;
    int mFPS;
    GLFWwindow* mWindow;
    std::chrono::steady_clock::time_point mLastFrameTime;
    std::map<std::string, Shader> mShaders;
    Camera mCamera;
    glm::vec2 mLastCursorPosition;

    void processInput();
    void draw();
    void scrollEvent(double xoffset, double yoffset);
    void mouseButtonEvent(int button, int action, int mods);
    void mousePositionEvent(double xpos, double ypos);

public:
    Renderer(): mWidth(1280), mHeight(800), mFPS(60), mWindow(nullptr),
      mLastFrameTime(std::chrono::steady_clock::now()),
      mCamera(glm::vec3{-1., 4., 3.}, glm::vec3{0., 2., 0.}) {}
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    ~Renderer();
    void addMesh(Mesh* mesh) { mMeshes.push_back(std::unique_ptr<Mesh>(mesh)); }
    void addMaterial(Material* material) { mMaterials.push_back(std::unique_ptr<Material>(material)); }
    void initialize();
    void renderLoop();
    void updateWindowSize(int width, int height);
};
