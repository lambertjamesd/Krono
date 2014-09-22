project(TowerDefense)

set (SOURCE_FILES
	TowerDefense.cpp

	GameLogic/SpinBehavior.cpp
)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${DEFERRED_RENDERING_INCLUDE}")
include_directories("${BASIC_GAME_ENGINE_INCLUDE}")

set (EXTRA_LIBS ${EXTRA_LIBS} 
	DeferredRendering
	BasicGameEngine
	freeimage)

if (USE_XLIB)
	set (EXTRA_LIBS ${EXTRA_LIBS} 
		X11
		GLU)
endif (USE_XLIB)

if (USE_OPENGL4_4)
	add_definitions(-DGLEW_BUILD)
	set (EXTRA_LIBS ${EXTRA_LIBS} 
		GL
		GLEW)
endif (USE_OPENGL4_4)

add_executable (TowerDefense ${SOURCE_FILES})
target_link_libraries (TowerDefense ${EXTRA_LIBS})

add_custom_target(
	Shaders
	COMMAND make
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Media/Shaders
)
add_dependencies(TowerDefense Shaders)

add_custom_target(
	EngineShaders
	COMMAND make
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Media/Engine/
)
add_dependencies(TowerDefense EngineShaders)
