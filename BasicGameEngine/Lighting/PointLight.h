#pragma once

#include "PositionalLight.h"

namespace kge
{

class PointLight : public PositionalLight
{
public:
	typedef std::shared_ptr<PointLight> Ptr;
	typedef std::weak_ptr<PointLight> Ref;

	PointLight(GameObject& parentGameObject);
	~PointLight(void);
	
	virtual void PreRender();
private:

	float mRadius;
};

}