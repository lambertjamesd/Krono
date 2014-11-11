#pragma once

#include "Component.h"
#include "Transform.h"
#include <vector>
#include <memory>
#include <functional>

namespace kge
{

class Scene;

class GameObject : public krono::Object
{
public:
	typedef std::shared_ptr<GameObject> Ptr;
	typedef std::weak_ptr<GameObject> Ref;
	
	typedef std::function<void (GameObject&)> IteratorCallback;

	virtual ~GameObject(void);

	size_t GetComponentCount() const;
	Component::Ref GetComponent(size_t index) const;

	template <typename T>
	std::weak_ptr<T> GetComponent() const
	{
		for (auto it = mComponents.cbegin(); it != mComponents.cend(); ++it)
		{
			std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(*it);
			if (result != NULL)
			{
				return result;
			}
		}

		return std::weak_ptr<T>();
	}

	const Transform::Ptr GetTransform() const;

	template <typename T>
	std::weak_ptr<T> AddComponent()
	{
		std::shared_ptr<T> result(new T(*this));
		result->mSelfReference = result;
		mComponents.push_back(result);
		return result;
	}

	Scene& GetScene();

	Ref GetWeakPointer() const;

	void VisitSelfAndDecendants(IteratorCallback callback);

	void Destroy();
private:
	friend class Scene;

	GameObject(Scene& parentScene);

	Scene& mScene;

	std::vector<Component::Ptr> mComponents;
	Transform::Ptr mTransform;
	Ref mSelfReference;
};

}

