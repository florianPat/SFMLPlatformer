#pragma once

#include <memory>
#include "EventManager.h"

class Component
{
protected:
	const int id;
	EventManager* eventManager;
public:
	Component(int id, EventManager* eventManager) : id(id), eventManager(eventManager) {};
	virtual void update(float dt) {};
	virtual void draw() {};
	int getId() const { return id; };
};