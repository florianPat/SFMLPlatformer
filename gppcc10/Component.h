#pragma once

#include <memory>

class Component
{
protected:
	const int id;
public:
	Component(int id) : id(id) {};
	virtual void update(float dt) {};
	virtual void draw() {};
	int getId() const { return id; };
};