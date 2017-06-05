#include "Physics.h"

Physics::Physics(PhysicsHandler* physicsHandler) : bodies(), physicsHandler(physicsHandler)
{
}

void Physics::update()
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		for (auto collisionIdIt = it->second.collisionIds.begin(); collisionIdIt != it->second.collisionIds.end(); ++collisionIdIt)
		{
			auto collideElement = bodies.find(*collisionIdIt);
			if (collideElement != bodies.end())
			{
				if (it->second.collidersInPointer)
				{
					if (collideElement->second.collidersInPointer)
					{
						for (auto colliders = collideElement->second.colliders.collidersPointer.begin(); colliders != collideElement->second.colliders.collidersPointer.end(); ++colliders)
						{
							for (auto bodyCollider = it->second.colliders.collidersPointer.begin(); bodyCollider != it->second.colliders.collidersPointer.end(); ++bodyCollider)
							{
								if ((*bodyCollider)->intersects(**colliders))
									physicsHandler(collideElement->second.id, it->second.id);
							}
						}
					}
					else
					{
						for (auto colliders = collideElement->second.colliders.collidersValue.begin(); colliders != collideElement->second.colliders.collidersValue.end(); ++colliders)
						{
							for (auto bodyCollider = it->second.colliders.collidersPointer.begin(); bodyCollider != it->second.colliders.collidersPointer.end(); ++bodyCollider)
							{
								if ((*bodyCollider)->intersects(*colliders))
									physicsHandler(collideElement->second.id, it->second.id);
							}
						}
					}
				}
				else
				{
					if (collideElement->second.collidersInPointer)
					{
						for (auto colliders = collideElement->second.colliders.collidersPointer.begin(); colliders != collideElement->second.colliders.collidersPointer.end(); ++colliders)
						{
							for (auto bodyCollider = it->second.colliders.collidersValue.begin(); bodyCollider != it->second.colliders.collidersValue.end(); ++bodyCollider)
							{
								if (bodyCollider->intersects(**colliders))
									physicsHandler(collideElement->second.id, it->second.id);
							}
						}
					}
					else
					{
						for (auto colliders = collideElement->second.colliders.collidersValue.begin(); colliders != collideElement->second.colliders.collidersValue.end(); ++colliders)
						{
							for (auto bodyCollider = it->second.colliders.collidersValue.begin(); bodyCollider != it->second.colliders.collidersValue.end(); ++bodyCollider)
							{
								if (bodyCollider->intersects(*colliders))
									physicsHandler(collideElement->second.id, it->second.id);
							}
						}
					}
				}
			}
		}
	}
}

void Physics::addElementPointer(std::string name, std::vector<sf::FloatRect*> colliders, std::vector<std::string> collisionId)
{
	PhysicElement physicsElement = {};
	physicsElement.id = name;
	physicsElement.collisionIds = collisionId;
	physicsElement.collidersInPointer = true;
	physicsElement.colliders.collidersPointer = colliders;

	bodies.emplace(name, physicsElement);
}

void Physics::addElementValue(std::string name, std::vector<sf::FloatRect> colliders, std::vector<std::string> collisionId)
{
	PhysicElement physicsElement = {};
	physicsElement.id = name;
	physicsElement.collisionIds = collisionId;
	physicsElement.collidersInPointer = false;
	physicsElement.colliders.collidersValue = colliders;

	bodies.emplace(name, physicsElement);
}