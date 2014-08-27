// TowerDefense.cpp : Defines the entry point for the application.
//

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
#include <chrono>
#include <thread>

#include <Krono.h>

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "GameObject/Renderer.h"
#include "GameObject/Camera.h"

#include "GameLogic/SpinBehavior.h"

using namespace std;
using namespace krono;

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

Compositor::Ptr BuildCompositor(Graphics::Ptr& graphics, ResourceManager::Ptr& resourceManager, ::Scene& scene)
{
	Compositor::Ptr compositor(new Compositor());
	
	compositor->AddRenderTarget("color", DataFormat(DataFormat::Float, 4));
	compositor->AddRenderTarget("normal", DataFormat(DataFormat::Float, 4));
	compositor->AddRenderTarget("depth", DataFormat(DataFormat::Depth32F, 1));
	compositor->AddRenderTarget("combined", DataFormat(DataFormat::Float, 4));

	compositor->SetClearColor("color", Colorf(0.1f, 0.4f, 0.7f, 1.0f));
	compositor->SetClearColor("normal", Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	compositor->SetClearDepth("depth", 1.0f);

	RenderSceneCompositeStage* renderScene;
	ScreenQuadCompositeStage* colorMap;

	CompositeStageConnections renderSceneConnections;
	renderSceneConnections.AddRenderTarget("color");
	renderSceneConnections.AddRenderTarget("normal");
	renderSceneConnections.SetDepthBuffer("depth");

	CompositeStageConnections colorMapConnections;
	colorMapConnections.AddRenderTarget("output0");
	colorMapConnections.AddRenderTargetInput("color");

	compositor->AddCompositeStage(CompositeStage::Ptr(renderScene = new RenderSceneCompositeStage(0)), renderSceneConnections);
	compositor->AddCompositeStage(CompositeStage::Ptr(colorMap = new ScreenQuadCompositeStage(resourceManager->GetPlane())), colorMapConnections);

	
	colorMap->GetStateParameters().SetVertexShader(resourceManager->LoadResource<VertexShader>("Media/Shaders/Bundle/ScreenComposite.shader"));
	colorMap->GetStateParameters().SetPixelShader(resourceManager->LoadResource<PixelShader>("Media/Shaders/Bundle/ColorMapper.shader"));
	colorMap->SetExpectedTargetInputCount(1);


	return compositor;
}

int main(int argc, char* argv[])
{
	Graphics::API api = Graphics::OpenGL;

	Auto<Graphics> graphics;
	Auto<Window> window = Window::Create(Vector2i(800, 600));

	if (window == NULL)
	{
		std::cerr << "Could not create window" << std::endl;
		return 1;
	}

	window->Show();

	Auto<VertexShader> vertexShader;
	Auto<PixelShader> pixelShader;

	Auto<ResourceManager> resourceManager;
	
	Auto<WindowRenderTarget> windowRenderTarget;
	Auto<DepthBuffer> depthBuffer;

	Auto<Mesh> meshTest;

	Auto<Texture2D> textureTest;
	
	Auto<Sampler> linearSampler;
	Auto<Sampler> pointSampler;

	
	graphics = Graphics::CreateGraphics(api);
	windowRenderTarget = graphics->CreateWindowRenderTarget(*window);
	resourceManager = Auto<ResourceManager>(new ResourceManager(graphics.get()));

	::Scene scene(graphics);

	GameObject::Ref objectReference = scene.CreateGameObject();
	objectReference.lock()->AddComponent<SpinBehavior>();
	Renderer::Ref renderer = objectReference.lock()->AddComponent<Renderer>();

	scene.GetRenderManager().SetDefaultCompositor(BuildCompositor(graphics, resourceManager, scene));

	GameObject::Ref cameraObject = scene.CreateGameObject();
	Camera::Ref camera = cameraObject.lock()->AddComponent<Camera>();
	unique_ptr<Lens> cameraLens(new PerspectiveLens(0.01f, 10.0f, Degreesf(90.0f)));
	camera.lock()->SetLens(cameraLens);

	try
	{

		depthBuffer = graphics->CreateDepthBuffer(window->GetSize(), DataFormat::Depth24);

		vertexShader = resourceManager->LoadResource<VertexShader>("Media/Shaders/Bundle/VertexShaderTest.shader");
		pixelShader = resourceManager->LoadResource<PixelShader>("Media/Shaders/Bundle/PixelShaderTest.shader");
		
		meshTest = resourceManager->LoadResource<Mesh>("Media/Meshes/Suzanne.obj#Suzanne");

		textureTest = resourceManager->LoadResource<Texture2D>("Media/Textures/Test.png");
		textureTest->GenerateMipmaps();

		SamplerDescription samplerDesc;

		linearSampler = graphics->CreateSampler(samplerDesc);

		samplerDesc.minFilter = InterpolationMode::Point;
		samplerDesc.magFilter = InterpolationMode::Point;
		samplerDesc.mipFilter = InterpolationMode::Point;

		pointSampler = graphics->CreateSampler(samplerDesc);

		Auto<Material> materialTest(new Material());
		materialTest->AddTechnique(0, Technique(vertexShader, pixelShader));
		materialTest->SetTexture(textureTest, 0, ShaderStage::PixelShader);
		
		Renderer::Ptr rendererPtr = renderer.lock();
		rendererPtr->SetMesh(meshTest);
		rendererPtr->SetMaterial(materialTest, 0);

		objectReference.lock()->GetTransform()->SetLocalScale(Vector3f(1.0f, 1.0f, 1.0f));
		
		RenderTargetConfiguration renderTarget;
		renderTarget.AddRenderTarget(windowRenderTarget);
		camera.lock()->SetRenderTargets(renderTarget);
		cameraObject.lock()->GetTransform()->SetLocalPosition(Vector3f(0.0f, 0.0f, -2.0f));
	}
	catch (Exception& exception)
	{
		const char* what = exception.what();
		std::cerr << what;
		exit(1);
	}

	graphics->SetSampler(pointSampler, 0, ShaderStage::PixelShader);
	graphics->SetSampler(linearSampler, 1, ShaderStage::PixelShader);
	
	chrono::time_point<chrono::system_clock> lastFrameTime = chrono::system_clock::now();

	chrono::duration<double> frameDuration(1.0 / 60.0);

	while (!window->IsClosed())
	{
		chrono::time_point<chrono::system_clock> frameTime = chrono::system_clock::now();

		chrono::duration<float> updateAmount = chrono::duration_cast<chrono::duration<float> >(frameTime - lastFrameTime);

		scene.GetUpdateManager().Update(updateAmount.count());
		scene.GetRenderManager().Render();

		windowRenderTarget->Present();

		window->Update(true);

		chrono::time_point<chrono::system_clock> endFrameTime = chrono::system_clock::now();
		chrono::duration<double> sleepTime = frameDuration - (endFrameTime - frameTime);
		
		if (sleepTime > chrono::duration<double>(0.0))
		{
			this_thread::sleep_for(chrono::milliseconds((long long)(sleepTime.count() * 1000.0)));
		}
		
		lastFrameTime = frameTime;
	}

	return 0;
}
