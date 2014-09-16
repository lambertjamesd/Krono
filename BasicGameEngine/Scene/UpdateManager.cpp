#include "UpdateManager.h"
#include <algorithm>
#include <cassert>

using namespace std;

Updatable::Updatable(UpdateManager& updateManager) :
	mUpdateManager(updateManager)
{
	mUpdateManager.AddUpdatable(this);
}

Updatable::~Updatable()
{
	mUpdateManager.RemoveUpdatable(this);
}

UpdateManager::UpdateManager(void) :
	mIsUpdating(false)
{

}

UpdateManager::~UpdateManager(void)
{

}

void UpdateManager::Update(float deltaTime)
{
	assert(!mIsUpdating && "Update called while already in update");

	mIsUpdating = true;

	for (auto it = mUpdateList.begin(); it != mUpdateList.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}

	mIsUpdating = false;
}

void UpdateManager::AddUpdatable(Updatable* target)
{
	mUpdateList.push_back(target);
}

void UpdateManager::RemoveUpdatable(Updatable* target)
{
	mUpdateList.erase(remove(mUpdateList.begin(), mUpdateList.end(), target), mUpdateList.end());
}