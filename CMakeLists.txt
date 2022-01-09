cmake_minimum_required(VERSION 3.3)
project(opengltest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(SOURCE_FILES main.cpp)
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

add_executable(opengltest main.cpp GameClasses/Util/libs/glad.c
        GameClasses/Basic/Shader.cpp
        GameClasses/Basic/Shader.h
        GameClasses/Util/libs/stb_image.h
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
        GameClasses/Base/Renderer.cpp
        GameClasses/Base/Renderer.h
        GameClasses/Entities/Entity.cpp
        GameClasses/Entities/Entity.h
        GameClasses/Base/World.cpp
        GameClasses/Base/World.h
        GameClasses/Util/Math.h
        GameClasses/Entities/Planet/Planet.cpp
        GameClasses/Entities/Planet/Planet.h
        GameClasses/Util/Cubesphere.cpp
        GameClasses/Util/Cubesphere.h
        GameClasses/Util/libs/imgui.cpp
        GameClasses/Util/libs/imgui.h
        GameClasses/Util/libs/imgui_impl_glfw.cpp
        GameClasses/Util/libs/imgui_impl_glfw.h
        GameClasses/Util/libs/imgui_widgets.cpp
        GameClasses/Util/libs/imgui_demo.cpp
        GameClasses/Util/libs/imgui_draw.cpp
        GameClasses/Util/libs/imgui_internal.h
        GameClasses/Util/libs/imgui_tables.cpp
        GameClasses/Util/libs/imgui_impl_opengl3.cpp
        GameClasses/Util/libs/imgui_impl_opengl3.h
        GameClasses/Entities/Planet/Atmosphere.cpp
        GameClasses/Entities/Planet/Atmosphere.h
        GameClasses/Entities/Planet/WaterSphere.cpp
        GameClasses/Entities/Planet/WaterSphere.h
        GameClasses/Base/EngineSettings.h)
target_link_libraries(opengltest glfw GL X11 pthread Xrandr Xi dl assimp ReactPhysics3D::ReactPhysics3D)