#include "renderer.h"
#include "exceptions.h"
#include <glad/glad.h>
#include <thread>
#ifdef MEMCHECK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

void Renderer::initialize()
{
    // GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    mWindow = glfwCreateWindow(mWidth, mHeight, "Playground", NULL, NULL);
    if (mWindow == NULL) {
        glfwTerminate();
        throw OpenGLException("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(mWindow);
    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw OpenGLException("Failed to initialize GLAD");
    }
    glfwSetWindowUserPointer(mWindow, this);
    // viewport setup
    glViewport(0, 0, mWidth, mHeight);
    mCamera.setAspectRatio(float(mWidth) / mHeight);
    glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height){
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->updateWindowSize(width, height);
    });
    // build shaders
    std::string vertexSource, fragmentSource;
    vertexSource = readFile("D:\\MyFiles\\games-202\\playground\\src\\shaders\\basic\\vertex.glsl");
    fragmentSource = readFile("D:\\MyFiles\\games-202\\playground\\src\\shaders\\basic\\fragment.glsl");
    std::vector<std::string> uniforms{"uModelViewMatrix", "uProjectionMatrix", "uTextureSample", "uKd"};
    std::vector<std::string> textures{"uSampler"};
    Shader basicShader(vertexSource, fragmentSource, uniforms, textures);
    mShaders["basic"] = std::move(basicShader);
}

void Renderer::renderLoop()
{
#ifdef MEMCHECK
    _CrtMemState s1, s2, diff;
#endif
    while(!glfwWindowShouldClose(mWindow)) {
#ifdef MEMCHECK
        _CrtMemCheckpoint(&s1);
        // _CrtMemDumpStatistics(&s1);
#endif
        auto currTime = std::chrono::steady_clock::now();
        long microsecPerLoop = 1000000 / mFPS;
        microsecPerLoop = (95 * microsecPerLoop) / 100; // give a little slack so we actually hit target fps
        auto elapsed = currTime - mLastFrameTime;
        if (std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() < microsecPerLoop) {
            std::this_thread::sleep_for(std::chrono::microseconds(microsecPerLoop) - elapsed);
        }
        mLastFrameTime = std::chrono::steady_clock::now();
        glfwPollEvents();
        processInput();
        draw();
        glfwSwapBuffers(mWindow);
#ifdef MEMCHECK
        _CrtMemCheckpoint(&s2);
        // _CrtMemDumpStatistics(&s2);
        _CrtMemDifference(&diff, &s1, &s2);
        _CrtMemDumpStatistics(&diff);
#endif
    }
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::processInput()
{
    if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);
}

void Renderer::updateWindowSize(int width, int height)
{
    mWidth = width;
    mHeight = height;
    mCamera.setAspectRatio(float(mWidth) / mHeight);
    glViewport(0, 0, mWidth, mHeight);
}

void Renderer::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto& m: mMeshes) {
        Mesh* mesh = m.get();
        mesh->draw(mCamera, mShaders["basic"]);
    }
}
