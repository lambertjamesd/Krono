project(TowerDefense)

set (SOURCE_FILES
	TowerDefense.cpp

	GameObject/Transform.cpp
	GameObject/Component.cpp
	GameObject/GameObject.cpp
)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${DEFERRED_RENDERING_INCLUDE}")

set (EXTRA_LIBS ${EXTRA_LIBS} 
	DeferredRendering
	freeimage)

add_executable (TowerDefense ${SOURCE_FILES})
target_link_libraries (TowerDefense ${EXTRA_LIBS})
