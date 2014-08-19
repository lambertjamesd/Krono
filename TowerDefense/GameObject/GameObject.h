#pragma once

#include "Component.h"
#include "Transform.h"
#include <vector>

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);

	template <typename T>
	T* GetComponent() const
	{
		for (auto it = mComponents.cbegin(); it != mComponents.cend(); ++it)
		{
			T* result = dynamic_cast<T*>(it->get());
			if (result != NULL)
			{
				return result;
			}
		}

		return NULL;
	}

	template <typename T>
	T* AddComponent()
	{
		T* result;
		mComponents.push_back(Component::Ptr(result = new T(this)));
		return result;
	}

private:
	std::vector<Component::Ptr> mComponents;
};

