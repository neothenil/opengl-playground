file(GLOB SOURCES *.h *.cpp)
list(APPEND BACKEND_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/deps/assimp/include"
    "${CMAKE_SOURCE_DIR}/deps/glad/include"
    "${CMAKE_SOURCE_DIR}/deps/glfw/include"
    "${CMAKE_SOURCE_DIR}/deps/glm"
    "${CMAKE_SOURCE_DIR}/deps/stb")
list(APPEND BACKEND_LIBS assimp glad glfw glm_static)

if(check_memory)
    add_definitions(-DMEMCHECK)
endif()

add_executable(main ${SOURCES})
target_include_directories(main PUBLIC "${BACKEND_INCLUDE_DIRS}")
target_compile_definitions(main PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(main PRIVATE "${BACKEND_LIBS}")
target_compile_features(main PRIVATE cxx_std_17)
