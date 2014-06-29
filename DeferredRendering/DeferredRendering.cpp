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

	Graphics::API api = Graphics::OpenGL;

	Auto<Graphics> graphics;
	Auto<VertexBuffer> vertexBuffer;
	Auto<Window> windowTest = Window::Create(Size(800, 600));
	Auto<ShaderProgram> shaderProgram;

	windowTest->Show();
	
	Auto<WindowRenderTarget> windowRenderTarget;

	try
	{
		graphics = Graphics::CreateGraphics(api);
		windowRenderTarget = graphics->CreateWindowRenderTarget(*windowTest);

		InputLayout bufferInputLayout;
		bufferInputLayout.AddAttribute(Attribute("POSITION", Attribute::Float, 3));


		Auto<VertexShader> vertexShader;
		Auto<FragmentShader> fragmentShader;

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
		
		shaderProgram = graphics->CreateShaderProgram(vertexShader, fragmentShader);
		shaderProgram->Use();

		vertexBuffer = graphics->CreateVertexBuffer(bufferInputLayout);

		VertexBufferIterator bufferWriter = vertexBuffer->Lock(3);
		bufferWriter.Write<Vector3f>(Vector3f(0.0f, 0.5f, 0.0f));
		bufferWriter.Write<Vector3f>(Vector3f(0.45f, -0.5, 0.0f));
		bufferWriter.Write<Vector3f>(Vector3f(-0.45f, -0.5f, 0.0f));
		vertexBuffer->Unlock();
	}
	catch (Exception& exception)
	{
		std::cerr << exception.what();
	}

	while (!windowTest->IsClosed())
	{
		RenderTarget *renderTargetPointer = windowRenderTarget.get();
		graphics->BindRenderTargets(1, &renderTargetPointer, NULL);
		graphics->SetViewport(Rectf(Vector2f(), windowTest->GetSize().CastTo<float>()), Rangef(0.0f, 1.0f));
		windowRenderTarget->Clear(Colorf(0.1f, 0.4f, 0.7f, 1.0f));

		shaderProgram->BindVertexBuffer(*vertexBuffer);
		vertexBuffer->Use();

		graphics->Draw(3, 0);

		windowRenderTarget->Present();

		windowTest->Update(false);
	}

	return 0;
}
 