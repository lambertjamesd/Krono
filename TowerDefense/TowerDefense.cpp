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
#include "GameLogic/FlyCamera.h"

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

GameObject::Ptr AddObject(const kge::Scene::Ptr& scene, const std::string& mesh, const std::string& material)
{
	GameObject::Ptr result = scene->CreateGameObject().lock();
	Renderer::Ptr renderer = result->AddComponent<Renderer>().lock();

	renderer->SetMesh(scene->GetResourceManager().LoadResource<Mesh>(mesh));
	renderer->SetMaterial(scene->GetResourceManager().LoadResource<Material>(material), 0);

	return result;
}

void AddEmmisiveSphere(const kge::Scene::Ptr& scene, const Vector3f& location, const Colorf& color)
{
	GameObject::Ptr emmisiveSphere = scene->CreateGameObject().lock();
	emmisiveSphere->GetTransform()->SetLocalScale(Vector3f(0.1f, 0.1f, 0.1f));
	emmisiveSphere->GetTransform()->SetLocalPosition(location);
	Renderer::Ptr lightRender = emmisiveSphere->AddComponent<Renderer>().lock();
	lightRender->SetMesh(scene->GetResourceManager().GetSphere());
	Material::Ptr material = scene->GetResourceManager().LoadResource<Material>("Media/Materials/Emmisive.json");
	material->SetVariable<Colorf>("emmisiveColor", color);
	lightRender->SetMaterial(material, 0);
	PointLight::Ptr sphereGlow = emmisiveSphere->AddComponent<PointLight>().lock();
	sphereGlow->SetRange(8.0f);
	sphereGlow->SetColor(color * 0.02f);
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
	
	Auto<Sampler> linearSampler;
	Auto<Sampler> pointSampler;

	kge::Scene::Ptr scene = game.CreateScene();

	GameObject::Ptr conrellBox = AddObject(scene, "Media/Meshes/CornellBox.obj#Cube", "Media/Materials/CornellBox.json");
	conrellBox->GetTransform()->SetLocalOrientation(Quaternionf(Vector3f(0.0f, 1.0f, 0.0f), Degreesf(180.0f)));

	GameObject::Ptr suzanne = scene->CreateGameObject().lock();
	suzanne->GetTransform()->SetLocalOrientation(Quaternionf(Vector3f(0.0f, 1.0f, 0.0f), Degreesf(225.0f)));
	suzanne->GetTransform()->SetLocalScale(Vector3f(0.6f, 0.6f, 0.6f));
	suzanne->GetTransform()->SetLocalPosition(Vector3f(0.0f, -1.0f, 0.0f));
	suzanne->AddComponent<SpinBehavior>();
	Renderer::Ref renderer = suzanne->AddComponent<Renderer>();

	scene->GetRenderManager().SetDefaultCompositor(resourceManager->LoadResource<Compositor>("Media/Compositor/DeferredRender.json"));

	GameObject::Ptr cameraObject = scene->CreateGameObject().lock();
	cameraObject->AddComponent<FlyCamera>();
	Camera::Ref camera = cameraObject->AddComponent<Camera>();
	unique_ptr<Lens> cameraLens(new PerspectiveLens(0.05f, 20.0f, Degreesf(90.0f)));
	camera.lock()->SetLens(cameraLens);

	GameObject::Ptr cubeTest = AddObject(scene, "Media/Meshes/Cube.obj#Cube", "Media/Materials/Cube.json");
	cubeTest->GetTransform()->SetLocalScale(Vector3f(0.1f, 0.1f, 0.1f));
	cubeTest->GetTransform()->SetLocalPosition(Vector3f(0.0f, -0.5f, -0.8f));
	SpinBehavior::Ptr cubeSpin = cubeTest->AddComponent<SpinBehavior>().lock();
	cubeSpin->SetAxis(Vector3f(1.0f, 0.0f, 0.0f));

	cubeSpin = cubeTest->AddComponent<SpinBehavior>().lock();
	cubeSpin->SetAxis(Vector3f(0.0f, 1.0f, 0.0f));
	cubeSpin->SetRotationRate(Degreesf(7.0f));

	{
		GameObject::Ptr lightObject = scene->CreateGameObject().lock();
		lightObject->GetTransform()->SetLocalPosition(Vector3f(0.0f, 0.9f, 0.0f));
		PointLight::Ptr pointLight = lightObject->AddComponent<PointLight>().lock();
		pointLight->SetRange(4.0f);
		pointLight->SetColor(Colorf(1.0f, 1.0f, 1.0f));
	}

	for (int i = 1; i <= 5; ++i)
	{
		AddEmmisiveSphere(scene, Vector3f(0.4f * i - 1.2f, -0.9f, -0.8f), Colorf(0.3f, 0.5f + i * 0.3f, 2.0f - i * 0.3f));
	}

	try
	{
		vertexShader = resourceManager->LoadResource<VertexShader>("Media/Shaders/Bundle/VertexShaderTest.shader");
		pixelShader = resourceManager->LoadResource<PixelShader>("Media/Shaders/Bundle/PixelShaderTest.shader");
		
		meshTest = resourceManager->LoadResource<Mesh>("Media/Meshes/Suzanne.obj#Suzanne");

		SamplerDescription samplerDesc;

		linearSampler = graphics->CreateSampler(samplerDesc);

		samplerDesc.minFilter = InterpolationMode::Point;
		samplerDesc.magFilter = InterpolationMode::Point;
		samplerDesc.mipFilter = InterpolationMode::Point;

		pointSampler = graphics->CreateSampler(samplerDesc);
		
		Renderer::Ptr rendererPtr = renderer.lock();
		rendererPtr->SetMesh(meshTest);
		rendererPtr->SetMaterial(resourceManager->LoadResource<Material>("Media/Materials/Suzanne.json"), 0);
		
		RenderTargetConfiguration renderTarget;
		renderTarget.AddRenderTarget(windowRenderTarget);
		camera.lock()->SetRenderTargets(renderTarget);
		cameraObject->GetTransform()->SetLocalPosition(Vector3f(0.0f, 0.0f, -2.0f));
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
