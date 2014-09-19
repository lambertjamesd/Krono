
#include "Game.h"

#include <thread>

namespace kge
{

using namespace std;
using namespace krono;

Game::Game(Graphics::API api, Vector2i windowSize, float targetFramerate) :

	mFrameDuration(1.0f / targetFramerate),
	mLastFrameTime(chrono::system_clock::now())
{
	Initialize(api, windowSize);
}

void Game::Initialize(Graphics::API api, Vector2i windowSize)
{
	mWindow = Window::Create(Vector2i(800, 600));

	mGraphics = Graphics::CreateGraphics(api);
	mResourceManager = ResourceManager::Ptr(new ResourceManager(mGraphics.get()));

	mWindow->Show();
	mWindowRenderTarget = mGraphics->CreateWindowRenderTarget(*mWindow);
}

void Game::MainLoop()
{
	while (!mWindow->IsClosed())
	{
		mWindow->Update(true);
		Update();
	}
}

void Game::Update()
{
	chrono::time_point<chrono::system_clock> frameTime = chrono::system_clock::now();
	chrono::duration<float> updateAmount = chrono::duration_cast<chrono::duration<float> >(frameTime - mLastFrameTime);

	if (mCurrentScene != NULL)
	{
		mCurrentScene->GetUpdateManager().Update(updateAmount.count());
		mCurrentScene->GetRenderManager().Render();
	}

	mWindowRenderTarget->Present();

	chrono::time_point<chrono::system_clock> endFrameTime = chrono::system_clock::now();
	chrono::duration<float> sleepTime = mFrameDuration - (endFrameTime - frameTime);
	
	if (sleepTime > chrono::duration<float>(0.0))
	{
		this_thread::sleep_for(chrono::milliseconds((long long)(sleepTime.count() * 1000.0f)));
	}
	
	mLastFrameTime = frameTime;
}

Scene::Ptr Game::CreateScene()
{
	return Scene::Ptr(new Scene(*this));
}

void Game::SetCurrentScene(const Scene::Ptr& scene)
{
	mCurrentScene = scene;
}

const krono::Graphics::Ptr& Game::GetGraphics() const
{
	return mGraphics;
}

const krono::ResourceManager::Ptr& Game::GetResourceManager() const
{
	return mResourceManager;
}

const krono::WindowRenderTarget::Ptr& Game::GetWindowRenderTarget() const
{
	return mWindowRenderTarget;
}

}
