#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include <unordered_map>

#define PHYSICS_HANDLER(name) void name(std::string elementId1, std::string elementId2)
typedef PHYSICS_HANDLER(PhysicsHandler);

class Physics
{
	struct PhysicElement
	{
		std::string id;
		std::vector<std::string> collisionIds;
		bool collidersInPointer;
		struct //TODO: Make this a union, if possible! ( First attempt was a fail ;) )
		{
			std::vector<sf::FloatRect*> collidersPointer;
			std::vector<sf::FloatRect> collidersValue;
		} colliders;
	};
private:
	std::unordered_map<std::string, PhysicElement> bodies;
	PhysicsHandler* physicsHandler;
public:
	Physics(PhysicsHandler* physicsHandler);
	void update();
	//Should be called, if the object is moving
	void addElementPointer(std::string name, std::vector<sf::FloatRect*> colliders, std::vector<std::string> collisionId = {});
	//Should be called if the object, is a static one
	void addElementValue(std::string name, std::vector<sf::FloatRect> colliders, std::vector<std::string> collisionId = {});
};