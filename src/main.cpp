#include <string>
#include <iostream>

#include "renderer.h"
#include "objloader.h"

int main(int argc, char const *argv[])
{
    const std::string path = "D:\\MyFiles\\games-202\\playground\\assets\\mary\\Marry.obj";
    Renderer renderer;
    renderer.initialize();
    ObjLoader::loadObjFile(renderer, path);
    renderer.renderLoop();
    return 0;
}
