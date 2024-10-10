#ifdef MEMCHECK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <string>
#include <iostream>

#include "renderer.h"
#include "objloader.h"

int main(int argc, char const *argv[])
{
    {
        const std::string path = "D:\\MyFiles\\games-202\\playground\\assets\\mary\\Marry.obj";
        Renderer renderer;
        renderer.initialize();
        ObjLoader::loadObjFile(renderer, path);
        renderer.renderLoop();
    }
#ifdef MEMCHECK
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}
