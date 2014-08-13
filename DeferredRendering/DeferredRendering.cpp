// CS455 Shader Example

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <memory>
#include <fstream>
#include <cerrno>
#include <iomanip>

#include "Interface\Window.h"
#include "Core\Exception.h"
#include "Core\Hash.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Interface\Graphics.h"

#include "Math\Matrix.h"

#include "Resource\ResourceManager.h"
#include "Resource\Mesh.h"
#include "Scene\Scene.h"
#include "Scene\SceneView.h"

using namespace std;

std::string ReadFileContents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	throw(errno);
}

int main(int argc, char** argv)
{
	/* Code use goal
	RenderEngine renderEngine;

	Compositor compositor;

	compositor.AddRenderTarget(RenderTargetDefinition("RenderTarget/tgDiffuse.json");
	compositor.AddRenderTarget(RenderTargetDefinition("RenderTarget/tgNormal.json");
	compositor.AddRenderTarget(RenderTargetDefinition("RenderTarget/tgDepth.json");
	
	compositor.AddRenderTarget(RenderTargetDefinition("RenderTarget/tgColor.json");
	compositor.AddRenderTarget(RenderTargetDefinition("RenderTarget/tgScreen.json");

	Scene scene;

	// Fills the tgDiffuse, tgNormal, and tgDepth buffers
	compositor.AddStage(RenderScenePass());
	// uses tgNormal, tgNormal, and tgDepth and appends to tgColor
	compositor.AddStage(RenderLightingPass());
	// uses tgColor and renders to tgScreen
	compositor.AddStage(HDRPass());
	
	SceneView sceneView(scene, camera, viewport);

	sceneView.render(compositor);

	*/

	Graphics::API api = Graphics::OpenGL;

	Auto<Graphics> graphics;
	Auto<Window> window = Window::Create(Vector2i(800, 600));
	Auto<VertexShader> vertexShader;
	Auto<PixelShader> fragmentShader;

	Auto<ResourceManager> resourceManager;

	window->Show();
	
	Auto<WindowRenderTarget> windowRenderTarget;
	Auto<DepthBuffer> depthBuffer;

	Auto<Mesh> meshTest;

	Auto<Texture2D> textureTest;

	Scene sceneTest;
	SceneView sceneView(sceneTest);

	sceneView.SetViewMatrix(TranslationMatrix(Vector3f(0.5f, 0.5f, 0.5f)) * ScaleMatrix(Vector3f(0.25f, 0.25f, 0.25f)));

	try
	{
		graphics = Graphics::CreateGraphics(api);
		resourceManager = Auto<ResourceManager>(new ResourceManager(graphics.get()));
		windowRenderTarget = graphics->CreateWindowRenderTarget(*window);

		depthBuffer = graphics->CreateDepthBuffer(window->GetSize(), DataFormat::Depth24);

		if (api == Graphics::DirectX11)
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\HLSL\\VertexShaderTest.cso"));
			fragmentShader = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\HLSL\\PixelShaderTest.cso"));
		}
		else
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\GLSL\\VertexShaderTest.vert"));
			fragmentShader = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\GLSL\\PixelShaderTest.frag"));
		}

		meshTest = resourceManager->LoadResource<Mesh>("Media/Meshes/Suzanne.obj#Suzanne");

		textureTest = resourceManager->LoadResource<Texture2D>("Media/Textures/Test.png");

		Entity* entityTest = sceneTest.CreateEntity();
		entityTest->SetMesh(meshTest);

		Auto<Material> materialTest(new Material());
		materialTest->AddTechnique(0, Technique(vertexShader, fragmentShader));
		entityTest->SetMaterial(materialTest, 0);
	}
	catch (Exception& exception)
	{
		std::cerr << exception.what();
		std::cin.get();
		exit(1);
	}

	graphics->SetTexture(textureTest, 0);

	while (!window->IsClosed())
	{
		vector<Auto<RenderTarget>> renderTargetArray;
		renderTargetArray.push_back(windowRenderTarget);
		windowRenderTarget->Clear(Colorf(0.1f, 0.4f, 0.7f, 1.0f));
		graphics->SetRenderTargets(renderTargetArray, DepthBuffer::Null);
		graphics->SetViewport(Rectf(Vector2f(), windowRenderTarget->GetSize()), Rangef(0.0f, 1.0f));

		sceneView.Render(*graphics);

		windowRenderTarget->Present();

		window->Update(false);
	}

	return 0;
}
 