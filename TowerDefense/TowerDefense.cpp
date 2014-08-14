// TowerDefense.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TowerDefense.h"
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

#include <Krono.h>
using namespace std;

std::string ReadFileContents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize((unsigned int)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	throw(errno);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	Graphics::API api = Graphics::DirectX11;

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