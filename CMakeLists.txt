cmake_minimum_required (VERSION 2.6)

set (Krono_VERSION_MAJOR 0)
set (Krono_VERSION_MINOR 5)

project (Krono)

set (LINUX UNIX && NOT APPLE)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
    add_definitions(-D_GLIBCXX_USE_NANOSLEEP)
endif()

option (USE_OPENGL4_4 "Build Krono with OpenGL support" ON)

if (UNIX)
	option (USE_XLIB "Build Krono with XLib support" ON)
else (UNIX)
	option (USE_XLIB "Build Krono with XLib support" OFF)
endif (UNIX)

if (WIN32)
	add_definitions(-DUSE_DX11)
	set (USE_DX11 ON)
	message ( STATUS "Building with DirectX11 support")
endif (WIN32)

if (USE_OPENGL4_4)
	add_definitions(-DUSE_OPENGL4_4)
	message ( STATUS "Building with OpenGL 4.4 support")
endif (USE_OPENGL4_4)

if (USE_XLIB)
	add_definitions(-DUSE_XLIB)
	message ( STATUS "Building with XLib support")
endif (USE_XLIB)

MACRO(INSTALL_HEADERS_WITH_DIRECTORY PREFIX HEADER_LIST)

FOREACH(HEADER ${${HEADER_LIST}})
STRING(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
INSTALL(FILES ${HEADER} DESTINATION include/${PREFIX}${DIR})
ENDFOREACH(HEADER)

ENDMACRO(INSTALL_HEADERS_WITH_DIRECTORY)


set (DEFERRED_RENDERING_INCLUDE ${PROJECT_SOURCE_DIR}/DeferredRendering)
set (BASIC_GAME_ENGINE_INCLUDE ${PROJECT_SOURCE_DIR}/BasicGameEngine)

add_subdirectory(ShaderBundler)
add_subdirectory(DeferredRendering)
add_subdirectory(BasicGameEngine)
add_subdirectory(TowerDefense)
add_custom_target(
	Shaders
	COMMAND make
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/TowerDefense/Media/Shaders
)
add_dependencies(TowerDefense Shaders)

