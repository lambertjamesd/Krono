#pragma once

#include "Component.h"
#include "Transform.h"
#include <vector>
#include <memory>

class Scene;

class GameObject
{
public:
	typedef std::shared_ptr<GameObject> Ptr;
	typedef std::weak_ptr<GameObject> Ref;

	~GameObject(void);

	template <typename T>
	std::weak_ptr<T> GetComponent() const
	{
		for (auto it = mComponents.cbegin(); it != mComponents.cend(); ++it)
		{
			std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(it);
			if (result != NULL)
			{
				return result;
			}
		}

		return std::weak_ptr<T>(NULL);
	}

	template <typename T>
	std::weak_ptr<T> AddComponent()
	{
		std::shared_ptr<T> result(new T(*this));
		mComponents.push_back(result);
		return result;
	}

	Scene& GetScene();
private:
	friend class Scene;

	GameObject(Scene* parentScene);

	std::vector<Component::Ptr> mComponents;
	Scene* mScene;
	Ref mSelfReference;
};

