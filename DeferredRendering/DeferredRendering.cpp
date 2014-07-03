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

	Graphics::API api = Graphics::DirectX11;

	Auto<Graphics> graphics;
	Auto<VertexBuffer> vertexBuffer;
	Auto<Window> window = Window::Create(Vector2i(800, 600));
	Auto<VertexShader> vertexShader;
	Auto<FragmentShader> fragmentShader;

	window->Show();
	
	Auto<WindowRenderTarget> windowRenderTarget;
	Auto<RenderTarget> offscreenTest;

	Auto<VertexBuffer> screenCoverQuad;

	try
	{
		graphics = Graphics::CreateGraphics(api);
		windowRenderTarget = graphics->CreateWindowRenderTarget(*window);


		offscreenTest = graphics->CreateOffscreenRenderTarget(window->GetSize(), DataFormat(DataFormat::UInt8, 4));

		if (api == Graphics::DirectX11)
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Shaders\\HLSL\\VertexShaderTest.cso"));
			fragmentShader = graphics->CreateFragmentShader(ReadFileContents("Shaders\\HLSL\\PixelShaderTest.cso"));
		}
		else
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Shaders\\GLSL\\VertexShaderTest.vert"));
			fragmentShader = graphics->CreateFragmentShader(ReadFileContents("Shaders\\GLSL\\PixelShaderTest.frag"));
		}
		
		InputLayout bufferInputLayout;
		bufferInputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
		bufferInputLayout.AddAttribute(Attribute("COLOR", DataFormat(DataFormat::Float, 4)));
		vertexBuffer = graphics->CreateVertexBuffer(bufferInputLayout);

		DataIterator bufferWriter = vertexBuffer->Lock(3);
		bufferWriter.Write<Vector3f>(Vector3f(0.0f, 0.5f, 0.0f));
		bufferWriter.Write<Colorf>(Colorf(1.0f, 0.0f, 1.0f, 1.0f));

		bufferWriter.Write<Vector3f>(Vector3f(0.45f, -0.5f, 0.0f));
		bufferWriter.Write<Colorf>(Colorf(1.0f, 1.0f, 0.0f, 1.0f));

		bufferWriter.Write<Vector3f>(Vector3f(-0.45f, -0.5f, 0.0f));
		bufferWriter.Write<Colorf>(Colorf(0.0f, 1.0f, 1.0f, 1.0f));
		vertexBuffer->Unlock();

		InputLayout screenCoverLayout;
		screenCoverLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
		screenCoverLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));
		screenCoverQuad = graphics->CreateVertexBuffer(screenCoverLayout);

		DataIterator screenIterator = screenCoverQuad->Lock(4);
		screenIterator.Write<Vector3f>(Vector3f(-1.0f, 1.0f, 0.0f));
		screenIterator.Write<Vector2f>(Vector2f(0.0f, 0.0f));
		screenIterator.Write<Vector3f>(Vector3f(1.0f, 1.0f, 0.0f));
		screenIterator.Write<Vector2f>(Vector2f(1.0f, 0.0f));
		screenIterator.Write<Vector3f>(Vector3f(1.0f, -1.0f, 0.0f));
		screenIterator.Write<Vector2f>(Vector2f(1.0f, 1.0f));
		screenIterator.Write<Vector3f>(Vector3f(-1.0f, -1.0f, 0.0f));
		screenIterator.Write<Vector2f>(Vector2f(0.0f, 1.0f));
		screenCoverQuad->Unlock();

	}
	catch (Exception& exception)
	{
		std::cerr << exception.what();
	}

	while (!window->IsClosed())
	{
		vector<Auto<RenderTarget>> renderTargetArray;
		renderTargetArray.push_back(offscreenTest);
		graphics->SetRenderTargets(renderTargetArray, Auto<DepthBuffer>(NULL));
		graphics->SetViewport(Rectf(Vector2f(), window->GetSize()), Rangef(0.0f, 1.0f));
		offscreenTest->Clear(Colorf(0.1f, 0.4f, 0.7f, 1.0f));

		graphics->SetVertexShader(vertexShader);
		graphics->SetFragmentShader(fragmentShader);
		graphics->SetVertexBuffer(vertexBuffer);

		graphics->Draw(3, 0);

		renderTargetArray[0] = windowRenderTarget;
		graphics->SetRenderTargets(renderTargetArray, Auto<DepthBuffer>(NULL));
		
		graphics->SetVertexBuffer(screenCoverQuad);

		windowRenderTarget->Present();

		window->Update(false);
	}

	return 0;
}
 