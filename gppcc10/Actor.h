#pragma once

#include <unordered_map>
#include "Component.h"
#include <memory>

class Actor
{
	friend class GameObjectManager;

	const int id;
	std::unordered_map<int, std::shared_ptr<Component>> components;
private:
	void update(float dt);
	void draw();
public:
	Actor(const int& id);
	void addComponent(std::shared_ptr<Component> component);
	void removeComponent(int componentId);
	//std::shared_ptr<T> getComponent(int componentId); //TODO: Make this a template!
	int getId() const { return id; };
};