cmake_minimum_required (VERSION 2.6)
project (Krono)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

option (USE_OPENGL "Build Krono with OpenGL support" ON)

if (WIN32)
	add_definitions(-DUSE_DX11)
	set (USE_DX11 ON)
endif (WIN32)

if (USE_OPENGL)
	add_definitions(-DUSE_OPENGL)
endif (USE_OPENGL)


set (DEFERRED_RENDERING_INCLUDE ${PROJECT_SOURCE_DIR}/DeferredRendering)

add_subdirectory(ShaderBundler)
add_subdirectory(DeferredRendering)
add_subdirectory(TowerDefense)
