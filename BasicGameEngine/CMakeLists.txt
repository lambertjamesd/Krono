project (BasicGameEngine)

set (SOURCE_FILES	
	GameObject/Behavior.cpp
	GameObject/Camera.cpp
	GameObject/Component.cpp
	GameObject/GameObject.cpp
	GameObject/Lens.cpp
	GameObject/Renderer.cpp
	GameObject/Transform.cpp

	Render/Renderable.cpp
	Render/RenderManager.cpp
	Render/RenderStage.cpp

	Scene/Scene.cpp
	Scene/UpdateManager.cpp
)

set (HEADER_FILES
	BasicGameEngine.h
	)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${DEFERRED_RENDERING_INCLUDE}")

add_library (BasicGameEngine ${SOURCE_FILES})

INSTALL_TARGETS(/lib BasicGameEngine)
INSTALL_HEADERS_WITH_DIRECTORY(BasicGameEngine/ HEADER_FILES)
