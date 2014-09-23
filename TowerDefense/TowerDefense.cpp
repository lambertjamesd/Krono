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
#include <BasicGameEngine.h>

#include "GameLogic/SpinBehavior.h"

using namespace std;
using namespace krono;
using namespace kge;

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

int main(int argc, char* argv[])
{
	Game game(Graphics::OpenGL, Vector2i(800, 600), 60.0f);

	Auto<Graphics> graphics = game.GetGraphics();
	Auto<ResourceManager> resourceManager = game.GetResourceManager();
	Auto<WindowRenderTarget> windowRenderTarget = game.GetWindowRenderTarget();

	Auto<VertexShader> vertexShader;
	Auto<PixelShader> pixelShader;

	Auto<Mesh> meshTest;

	Auto<Texture2D> textureTest;
	
	Auto<Sampler> linearSampler;
	Auto<Sampler> pointSampler;

	kge::Scene::Ptr scene = game.CreateScene();

	GameObject::Ref objectReference = scene->CreateGameObject();
	objectReference.lock()->AddComponent<SpinBehavior>();
	Renderer::Ref renderer = objectReference.lock()->AddComponent<Renderer>();

	scene->GetRenderManager().SetDefaultCompositor(resourceManager->LoadResource<Compositor>("Media/Compositor/DeferredRender.json"));

	GameObject::Ref cameraObject = scene->CreateGameObject();
	Camera::Ref camera = cameraObject.lock()->AddComponent<Camera>();
	unique_ptr<Lens> cameraLens(new PerspectiveLens(0.5f, 20.0f, Degreesf(90.0f)));
	camera.lock()->SetLens(cameraLens);

	{
		GameObject::Ref lightObject = scene->CreateGameObject();
		lightObject.lock()->GetTransform()->SetLocalPosition(Vector3f(1.0f, 0.0f, -3.0f));
		PointLight::Ptr pointLight = lightObject.lock()->AddComponent<PointLight>().lock();
		pointLight->SetRange(4.0f);
		pointLight->SetColor(Colorf(0.5f, 0.6f, 10.9f));
	}
	
	{
		GameObject::Ref lightObject = scene->CreateGameObject();
		lightObject.lock()->GetTransform()->SetLocalPosition(Vector3f(-4.0f, -1.0f, 4.0f));
		PointLight::Ptr pointLight = lightObject.lock()->AddComponent<PointLight>().lock();
		pointLight->SetRange(6.0f);
		pointLight->SetColor(Colorf(0.7f, 0.6f, 0.3f));
	}

	try
	{
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
		
		Renderer::Ptr rendererPtr = renderer.lock();
		rendererPtr->SetMesh(meshTest);
		rendererPtr->SetMaterial(resourceManager->LoadResource<Material>("Media/Materials/Suzanne.json"), 0);

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

	game.SetCurrentScene(scene);
	
	game.MainLoop();

	return 0;
}
