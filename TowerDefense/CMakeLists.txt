project(TowerDefense)

set (SOURCE_FILES
	TowerDefense.cpp

	GameObject/Component.cpp
	GameObject/GameObject.cpp
	GameObject/Renderer.cpp
	GameObject/Transform.cpp

	Render/Renderable.cpp
	Render/RenderManager.cpp

	Scene/Scene.cpp
)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${DEFERRED_RENDERING_INCLUDE}")

set (EXTRA_LIBS ${EXTRA_LIBS} 
	DeferredRendering
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
