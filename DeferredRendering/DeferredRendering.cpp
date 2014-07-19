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

struct MatrixStructure
{
	Matrix4f projection;
	Matrix4f modelView;
};

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
	Auto<PixelShader> fragmentShader;

	Auto<ResourceManager> resourceManager;
	
	Auto<VertexShader> copyVertex;
	Auto<PixelShader> copyPixel;

	window->Show();
	
	Auto<WindowRenderTarget> windowRenderTarget;
	Auto<RenderTarget> offscreenTest;
	Auto<DepthBuffer> depthTest;

	Auto<VertexBuffer> screenCoverQuad;
	Auto<IndexBuffer> screenCoverIndex;
	Auto<Texture> offscreenTexture;
	Auto<Texture> depthTexture;

	Auto<ConstantBuffer> constantBuffer;

	Auto<Mesh> meshTest;

	MatrixStructure matrices;

	matrices.projection = Matrix4f::Identity();
	matrices.modelView = Matrix4f::Identity();
	
	matrices.projection.At(0, 0) = 0.25f;
	matrices.projection.At(1, 1) = 0.25f;
	matrices.projection.At(2, 2) = 0.25f;

	matrices.projection.At(0, 3) = 0.5f;
	matrices.projection.At(1, 3) = 0.5f;
	matrices.projection.At(2, 3) = 0.5f;

	try
	{
		graphics = Graphics::CreateGraphics(api);
		resourceManager = Auto<ResourceManager>(new ResourceManager(graphics.get()));
		windowRenderTarget = graphics->CreateWindowRenderTarget(*window);

		offscreenTest = graphics->CreateOffscreenRenderTarget(window->GetSize(), DataFormat(DataFormat::Float, 4));
		offscreenTexture = offscreenTest->GetTexture();

		depthTest = graphics->CreateDepthBuffer(window->GetSize(), DataFormat::Depth24S8);
		depthTexture = depthTest->GetTexture();

		if (api == Graphics::DirectX11)
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\HLSL\\VertexShaderTest.cso"));
			fragmentShader = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\HLSL\\PixelShaderTest.cso"));
			
			copyVertex = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\HLSL\\ScreenCopyVert.cso"));
			copyPixel = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\HLSL\\ScreenCopyPix.cso"));
		}
		else
		{
			vertexShader = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\GLSL\\VertexShaderTest.vert"));
			fragmentShader = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\GLSL\\PixelShaderTest.frag"));
			
			copyVertex = graphics->CreateVertexShader(ReadFileContents("Media\\Shaders\\GLSL\\ScreenCopy.vert"));
			copyPixel = graphics->CreatePixelShader(ReadFileContents("Media\\Shaders\\GLSL\\ScreenCopy.frag"));
		}
		
		InputLayout bufferInputLayout;
		bufferInputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
		bufferInputLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));
		vertexBuffer = graphics->CreateVertexBuffer(bufferInputLayout);

		DataIterator bufferWriter = vertexBuffer->Lock(6);
		bufferWriter.Write<Vector3f>(Vector3f(0.0f, 0.5f, 0.5f));
		bufferWriter.Write<Vector2f>(Vector2f(1.0f, 0.0f));

		bufferWriter.Write<Vector3f>(Vector3f(0.45f, -0.5f, 0.0f));
		bufferWriter.Write<Vector2f>(Vector2f(1.0f, 1.0f));

		bufferWriter.Write<Vector3f>(Vector3f(-0.45f, -0.5f, 1.0f));
		bufferWriter.Write<Vector2f>(Vector2f(0.0f, 1.0f));
		
		bufferWriter.Write<Vector3f>(Vector3f(0.0f, 0.5f, 0.5f));
		bufferWriter.Write<Vector2f>(Vector2f(1.0f, 0.0f));

		bufferWriter.Write<Vector3f>(Vector3f(0.45f, -0.5f, 1.0f));
		bufferWriter.Write<Vector2f>(Vector2f(1.0f, 1.0f));

		bufferWriter.Write<Vector3f>(Vector3f(-0.45f, -0.5f, 0.0f));
		bufferWriter.Write<Vector2f>(Vector2f(0.0f, 1.0f));
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

		screenCoverIndex = graphics->CreateIndexBuffer(IndexBuffer::UInt16);
		DataIterator indexInterator = screenCoverIndex->Lock(6);
		indexInterator.Write<short>(0);
		indexInterator.Write<short>(1);
		indexInterator.Write<short>(2);
		
		indexInterator.Write<short>(2);
		indexInterator.Write<short>(3);
		indexInterator.Write<short>(0);
		screenCoverIndex->Unlock();

		ConstantBufferLayout bufferLayout;
		bufferLayout.MarkProjectionMatrix(offsetof(MatrixStructure, projection));

		constantBuffer = graphics->CreateConstantBuffer(bufferLayout);

		meshTest = resourceManager->LoadResource<Mesh>("Media/Meshes/Suzanne.obj#Suzanne");
	}
	catch (Exception& exception)
	{
		std::cerr << exception.what();
	}

	while (!window->IsClosed())
	{
		constantBuffer->Set<MatrixStructure>(matrices);
		graphics->SetConstantBuffer(constantBuffer, 0);

		vector<Auto<RenderTarget>> renderTargetArray;
		renderTargetArray.push_back(offscreenTest);
		graphics->SetRenderTargets(renderTargetArray, depthTest);
		graphics->SetViewport(Rectf(Vector2f(), window->GetSize()), Rangef(0.0f, 1.0f));
		offscreenTest->Clear(Colorf(0.1f, 0.4f, 0.7f, 1.0f));
		depthTest->Clear(1.0f);

		graphics->SetVertexShader(vertexShader);
		graphics->SetPixelShader(fragmentShader);
		graphics->SetVertexBuffer(vertexBuffer);
		graphics->SetIndexBuffer(IndexBuffer::Null);

		graphics->Draw(6, 0);

		meshTest->GetSubMesh(0)->Render(*graphics);

		renderTargetArray[0] = windowRenderTarget;
		graphics->SetRenderTargets(renderTargetArray, DepthBuffer::Null);
		
		graphics->SetVertexShader(copyVertex);
		graphics->SetPixelShader(copyPixel);
		graphics->SetVertexBuffer(screenCoverQuad);
		graphics->SetIndexBuffer(screenCoverIndex);

		graphics->SetTexture(offscreenTexture, 0);
		graphics->SetTexture(depthTexture, 1);
		
		graphics->DrawIndexed(6, 0);

		windowRenderTarget->Present();

		window->Update(false);
	}

	return 0;
}
 