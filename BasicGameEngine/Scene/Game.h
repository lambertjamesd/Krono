#pragma once

#include <chrono>
#include <Krono.h>
#include "Scene.h"

namespace kge
{

class Game
{
public:
	Game(krono::Graphics::API api, krono::Vector2i windowSize, float targetFramerate);

	void MainLoop();
	void Update();

	Scene::Ptr CreateScene();

	void SetCurrentScene(const Scene::Ptr& scene);

	const krono::Graphics::Ptr& GetGraphics() const;
	const krono::ResourceManager::Ptr& GetResourceManager() const;
	const krono::WindowRenderTarget::Ptr& GetWindowRenderTarget() const;
private:
	void Initialize(krono::Graphics::API api, krono::Vector2i windowSize);

	krono::Window::Ptr mWindow;
	krono::WindowRenderTarget::Ptr mWindowRenderTarget;

	krono::Graphics::Ptr mGraphics;
	krono::ResourceManager::Ptr mResourceManager;

	Scene::Ptr mCurrentScene;

	std::chrono::duration<float> mFrameDuration;
	std::chrono::time_point<std::chrono::system_clock> mLastFrameTime;
};

}
