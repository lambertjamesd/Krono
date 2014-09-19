#include "Light.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
namespace kge
{
Light::Light(GameObject& parentGameObject) :
	Component(parentGameObject)
{

}


Light::~Light(void)
{

}
}
