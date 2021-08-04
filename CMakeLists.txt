cmake_minimum_required(VERSION 3.3)
project(opengltest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(SOURCE_FILES main.cpp)
add_subdirectory(libs/imgui)
include_directories(${PROJECT_SOURCE_DIR})
target_link_libraries(${APP_TARGET} imgui)
find_package(ReactPhysics3D REQUIRED)
if (assimp_FOUND)
    set(ASSIMP_LIBRARY "assimp")
    add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
    set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "${ASSIMP_LIBRARY_DIRS}/libassimp.so")
endif(assimp_FOUND)
if(WIN32)
    list (APPEND CMAKE_PREFIX_PATH "C:\\Program Files(x86)\\ReactPhysics3D")
elseif (APPLE)
    list (APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/ReactPhysics3D")
endif ()

add_executable(opengltest main.cpp libs/glad.c
        GameClasses/Basic/Shader.cpp
        GameClasses/Basic/Shader.h
        libs/stb_image.h
        GameClasses/Basic/Camera.cpp
        GameClasses/Basic/Camera.h
        GameClasses/Basic/Mesh.cpp
        GameClasses/Basic/Mesh.h
        GameClasses/Basic/Model.cpp
        GameClasses/Basic/Model.h
        GameClasses/Entities/Light.cpp
        GameClasses/Entities/Light.h
        GameClasses/Util/XmlParser.cpp
        GameClasses/Util/XmlParser.h
        GameClasses/Util/BasicShapeBuilder.h
        GameClasses/Renderer.cpp
        GameClasses/Renderer.h
        GameClasses/Entities/Entity.cpp
        GameClasses/Entities/Entity.h
        GameClasses/World.cpp
        GameClasses/World.h
        GameClasses/Entities/PhysicsObject.cpp
        GameClasses/Entities/PhysicsObject.h
        GameClasses/Entities/BasicTerrain.cpp
        GameClasses/Entities/BasicTerrain.h
        GameClasses/Util/Math.h
        GameClasses/Entities/BasicWater.cpp
        GameClasses/Entities/BasicWater.h
        GameClasses/Entities/Planet/Planet.cpp
        GameClasses/Entities/Planet/Planet.h
        GameClasses/Entities/Planet/Icosphere.cpp
        GameClasses/Entities/Planet/Icosphere.h
        GameClasses/Entities/Planet/TerrainChunk.cpp
        GameClasses/Entities/Planet/TerrainChunk.h
        GameClasses/Entities/Planet/Cubesphere.cpp
        GameClasses/Entities/Planet/Cubesphere.h
        libs/imgui.cpp
        libs/imgui.h
        libs/imgui_impl_glfw.cpp
        libs/imgui_impl_glfw.h
        libs/imgui_widgets.cpp
        libs/imgui_demo.cpp
        libs/imgui_draw.cpp
        libs/imgui_internal.h
        libs/imgui_tables.cpp
        libs/imgui_impl_opengl3.cpp
        libs/imgui_impl_opengl3.h)
target_link_libraries(opengltest glfw GL X11 pthread Xrandr Xi dl assimp ReactPhysics3D::ReactPhysics3D)