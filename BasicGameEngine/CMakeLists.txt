project (BasicGameEngine)

set (SOURCE_FILES	
	GameObject/Behavior.cpp
	GameObject/Camera.cpp
	GameObject/Component.cpp
	GameObject/GameObject.cpp
	GameObject/Lens.cpp
	GameObject/Renderer.cpp
	GameObject/Transform.cpp

	Lighting/Light.cpp
	Lighting/DirectionalLight.cpp
	Lighting/PositionalLight.cpp
	Lighting/PointLight.cpp

	Render/Renderable.cpp
	Render/RenderManager.cpp
	Render/RenderStage.cpp

	Scene/Scene.cpp
	Scene/Game.cpp
	Scene/UpdateManager.cpp
)

set (HEADER_FILES
	BasicGameEngine.h

	GameObject/Behavior.h
	GameObject/Camera.h
	GameObject/Component.h
	GameObject/GameObject.h
	GameObject/Lens.h
	GameObject/Renderer.h
	GameObject/Transform.h

	Lighting/Light.h
	Lighting/DirectionalLight.h
	Lighting/PositionalLight.h
	Lighting/PointLight.h

	Render/Renderable.h
	Render/RenderManager.h
	Render/RenderStage.h

	Scene/Scene.h
	Scene/Game.h
	Scene/UpdateManager.h
	)

set (EXTRA_LIBS ${EXTRA_LIBS} 
	DeferredRendering
	)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${DEFERRED_RENDERING_INCLUDE}")

add_library (BasicGameEngine ${SOURCE_FILES})
target_link_libraries (BasicGameEngine ${EXTRA_LIBS})

INSTALL_TARGETS(/lib BasicGameEngine)
INSTALL_HEADERS_WITH_DIRECTORY(BasicGameEngine/ HEADER_FILES)
