file(GLOB SOURCES *.h *.cpp)
list(APPEND BACKEND_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/deps/assimp/include"
    "${CMAKE_SOURCE_DIR}/deps/glad/include"
    "${CMAKE_SOURCE_DIR}/deps/glfw/include"
    "${CMAKE_SOURCE_DIR}/deps/glm")
list(APPEND BACKEND_LIBS assimp glad glfw glm_static)

add_executable(main ${SOURCES})
target_include_directories(main PUBLIC "${BACKEND_INCLUDE_DIRS}")
target_link_libraries(main PRIVATE "${BACKEND_LIBS}")
