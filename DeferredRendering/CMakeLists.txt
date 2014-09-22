project (DeferredRendering)

set (SOURCE_FILES
	Compositor/CompositeStage.cpp
	Compositor/Compositor.cpp
	Compositor/ScreenQuadCompositeStage.cpp
	Compositor/RenderSceneCompositeStage.cpp
	Compositor/LightingCompositeStage.cpp
	Compositor/RenderState.cpp
	Compositor/RenderStateParameters.cpp
	Compositor/RenderTargetConfiguration.cpp
	Compositor/RenderTargetDatabase.cpp
	Interface/Texture.cpp
	Interface/BlendState.cpp
	Interface/IndexBuffer.cpp
	Interface/Shader.cpp
	Interface/ShaderInputLayout.cpp
	Interface/VertexBuffer.cpp
	Interface/Graphics.cpp
	Interface/RasterizerState.cpp
	Interface/RenderTarget.cpp
	Interface/DataFormat.cpp
	Interface/Sampler.cpp
	Interface/InvalidEnumException.cpp
	Interface/ConstantBuffer.cpp
	Interface/MappedConstantBuffer.cpp
	Interface/DepthBuffer.cpp
	Interface/DepthState.cpp
	Interface/InputLayout.cpp
	Interface/UnsupportedFormatException.cpp
	Interface/Window.cpp
	Interface/Texture2D.cpp
	Math/Rect.cpp
	Math/Math.cpp
	Math/Matrix.cpp
	Math/Range.cpp
	Resource/Compositor/CompositeStageLoader.cpp
	Resource/Compositor/CompositorLoader.cpp
	Resource/ResourceLoader.cpp
	Resource/Mesh/MeshGroupLoader.cpp
	Resource/Mesh/Mesh.cpp
	Resource/Mesh/MeshGroup.cpp
	Resource/Mesh/SubMesh.cpp
	Resource/Mesh/MeshLoader.cpp
	Resource/Mesh/ObjMeshGroupLoader.cpp
	Resource/Technique.cpp
	Resource/FileHelper.cpp
	Resource/FormatException.cpp
	Resource/GeometryCache.cpp
	Resource/Material.cpp
	Resource/MaterialLoader.cpp
	Resource/NotFoundException.cpp
	Resource/TextureLoader.cpp
	Resource/Texture2DLoader.cpp
	Resource/LoadException.cpp
	Resource/SamplerLoader.cpp
	Resource/ShaderLoader.cpp
	Resource/Resource.cpp
	Resource/ResourceManager.cpp
	Resource/JSON/JsonDocumentLoader.cpp
	Resource/JSON/JsonDocument.cpp
	Resource/JSON/JsonTypeHelper.cpp
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
		DX11/DX11BlendState.cpp
		DX11/DX11DepthState.cpp
		DX11/DX11IndexBuffer.cpp
		DX11/DX11Shader.cpp
		DX11/DX11OffscreenRenderTarget.cpp
		DX11/DX11Graphics.cpp
		DX11/DX11RasterizerState.cpp
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
		OpenGL/OpenGLBlendState.cpp
		OpenGL/OpenGLRasterizerState.cpp
		OpenGL/OpenGLRenderTarget.cpp
		OpenGL/OpenGLFramebuffer.cpp
		OpenGL/OpenGLObjectID.cpp
		OpenGL/OpenGLFBODatabase.cpp
		OpenGL/OpenGLTexture2D.cpp
		OpenGL/OpenGLGraphics.cpp
		OpenGL/OpenGLSampler.cpp
		OpenGL/OpenGLIndexBuffer.cpp
		OpenGL/OpenGLDepthBuffer.cpp
		OpenGL/OpenGLDepthState.cpp
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

set (HEADER_FILES
	Krono.h 
	Interface/Graphics.h 
	Interface/VertexBuffer.h
	Interface/InputLayout.h 
	Core/Hash.h 
	Core/Types.h 
	Interface/DataFormat.h 
	Core/Types.h 
	Core/Memory.h 
	Interface/Shader.h 
	Interface/ShaderInputLayout.h
	Core/Object.h 
	Interface/Window.h 
	Math/Vector2.h 
	Math/Math.h 
	Interface/RenderTarget.h 
	Interface/Color.h Interface/Texture2D.h 
	Interface/Texture.h 
	Interface/DepthBuffer.h 
	Math/Rect.h 
	Math/Range.h 
	Interface/IndexBuffer.h 
	Interface/ConstantBuffer.h 
	Interface/MappedConstantBuffer.h 
	Interface/Sampler.h 
	Interface/DepthState.h 
	Interface/BlendState.h 
	Interface/RasterizerState.h 
	Resource/ResourceManager.h 
	Resource/ResourceLoader.h 
	Resource/Resource.h 
	Resource/LoadException.h 
	Core/Exception.h 
	Resource/GeometryCache.h 
	Resource/Mesh/Mesh.h 
	Collide/BoundingBox.h 
	Math/Vector3.h 
	Resource/Mesh/SubMesh.h 
	Resource/Material.h 
	Resource/Technique.h 
	Compositor/RenderStateParameters.h 
	Resource/FileHelper.h 
	Math/Matrix.h 
	Math/Vector4.h 
	Math/Quaternion.h 
	Math/Angle.h 
	Scene/Scene.h 
	Scene/SceneIndex.h 
	Collide/Frustrum.h 
	Collide/Plane.h 
	Scene/Entity.h 
	Scene/SceneView.h 
	Compositor/Compositor.h 
	Compositor/CompositeStage.h 
	Compositor/RenderState.h 
	Compositor/RenderTargetDatabase.h 
	Compositor/RenderTargetConfiguration.h 
	Compositor/RenderSceneCompositeStage.h 
	Compositor/ScreenQuadCompositeStage.h

	)

include_directories("${PROJECT_SOURCE_DIR}")

add_library (DeferredRendering ${SOURCE_FILES})

INSTALL_TARGETS(/lib DeferredRendering)
INSTALL_HEADERS_WITH_DIRECTORY(Krono/ HEADER_FILES)
