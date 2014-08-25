project (DeferredRendering)

set (SOURCE_FILES
	Compositor/CompositeStage.cpp
	Compositor/Compositor.cpp
	Compositor/RenderSceneCompositeStage.cpp
	Compositor/RenderState.cpp
	Compositor/RenderStateParameters.cpp
	Compositor/RenderTargetConfiguration.cpp
	Compositor/RenderTargetDatabase.cpp
	Interface/Texture.cpp
	Interface/IndexBuffer.cpp
	Interface/Shader.cpp
	Interface/VertexBuffer.cpp
	Interface/Graphics.cpp
	Interface/RenderTarget.cpp
	Interface/DataFormat.cpp
	Interface/Sampler.cpp
	Interface/InvalidEnumException.cpp
	Interface/ConstantBuffer.cpp
	Interface/DepthBuffer.cpp
	Interface/InputLayout.cpp
	Interface/UnsupportedFormatException.cpp
	Interface/Window.cpp
	Interface/Texture2D.cpp
	Math/Rect.cpp
	Math/Math.cpp
	Math/Matrix.cpp
	Math/Range.cpp
	Resource/ResourceLoader.cpp
	Resource/Mesh/MeshGroupLoader.cpp
	Resource/Mesh/Mesh.cpp
	Resource/Mesh/MeshGroup.cpp
	Resource/Mesh/SubMesh.cpp
	Resource/Mesh/MeshLoader.cpp
	Resource/Technique.cpp
	Resource/FormatException.cpp
	Resource/ObjMeshGroupLoader.cpp
	Resource/Material.cpp
	Resource/NotFoundException.cpp
	Resource/Texture2DLoader.cpp
	Resource/LoadException.cpp
	Resource/ShaderLoader.cpp
	Resource/Resource.cpp
	Resource/ResourceManager.cpp
	Resource/JSON/JsonDocumentLoader.cpp
	Resource/JSON/JsonDocument.cpp
	Core/Hash.cpp
	Core/Exception.cpp
	Core/Object.cpp
	Collide/Plane.cpp
	Collide/BoundingBox.cpp
	Collide/Frustrum.cpp
	Scene/Scene.cpp
	Scene/SceneIndexArray.cpp
	Scene/SceneIndex.cpp
	Scene/Entity.cpp
	Scene/SceneView.cpp
	JSON/json.cpp
)

if (WIN32)
	set (SOURCE_FILES
		${SOURCE_FILES}
		Windows/WindowsWindow.cpp
	)

endif(WIN32)

if (USE_XLIB)
	set (SOURCE_FILES
		${SOURCE_FILES}
		Linux/XLibWindow.cpp
	)
endif (USE_XLIB)

if (USE_DX11)
	set (SOURCE_FILES
		${SOURCE_FILES}
		DX11/DX11IndexBuffer.cpp
		DX11/DX11Shader.cpp
		DX11/DX11OffscreenRenderTarget.cpp
		DX11/DX11Graphics.cpp
		DX11/IDX11Resource.cpp
		DX11/DX11RenderTarget.cpp
		DX11/DX11VertexBuffer.cpp
		DX11/DX11Texture2D.cpp
		DX11/DX11Sampler.cpp
		DX11/DX11WindowRenderTarget.cpp
		DX11/DX11ConstantBuffer.cpp
		DX11/HResultException.cpp
		DX11/DX11DepthBuffer.cpp
	)
endif (USE_DX11)

if (USE_OPENGL4_4)
	set (SOURCE_FILES
		${SOURCE_FILES}
		OpenGL/OpenGLTexture.cpp
		OpenGL/OpenGLRenderTarget.cpp
		OpenGL/OpenGLFramebuffer.cpp
		OpenGL/OpenGLObjectID.cpp
		OpenGL/OpenGLFBODatabase.cpp
		OpenGL/OpenGLTexture2D.cpp
		OpenGL/OpenGLGraphics.cpp
		OpenGL/OpenGLSampler.cpp
		OpenGL/OpenGLIndexBuffer.cpp
		OpenGL/OpenGLDepthBuffer.cpp
		OpenGL/OpenGLWindowRenderTarget.cpp
		OpenGL/OpenGLShader.cpp
		OpenGL/OpenGLVertexBuffer.cpp
		OpenGL/OpenGLTextureStorage.cpp
		OpenGL/OpenGLConstantBuffer.cpp
		OpenGL/OpenGLConstantBufferStorage.cpp
		OpenGL/OpenGLShaderDatabase.cpp
		OpenGL/OpenGLShaderProgram.cpp
		OpenGL/OpenGLOffscreenRenderTarget.cpp
	)
endif (USE_OPENGL4_4)

include_directories("${PROJECT_SOURCE_DIR}")

add_library (DeferredRendering ${SOURCE_FILES})
