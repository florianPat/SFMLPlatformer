#include "Actor.h"

Actor::Actor(const int& id) : components(), id(id)
{
}

void Actor::addComponent(std::shared_ptr<Component> component)
{
	if(components.find(component->getId()) == components.end())
		components.emplace(component->getId(), component);
}

void Actor::removeComponent(int componentId)
{
	auto it = components.find(componentId);

	if (it != components.end())
		components.erase(it);
}

void Actor::update(float dt)
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->update(dt);
	}
}

void Actor::draw()
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->draw();
	}
}

//TODO: Firgure out how to make this a template function
/*template <T>
std::shared_ptr<T> Actor::getComponent(int componentId)
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		if (it->second->getId() == componentId)
			return *it;
	}
	return nullptr;
}
*/