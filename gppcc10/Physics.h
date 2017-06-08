#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

class Physics
{
	struct PhysicElement
	{
		std::string id;
		std::vector<std::string> collisionIds;
		bool collidersInPointer;
		sf::FloatRect shoes;
		sf::FloatRect leftBody, rightBody;
		struct //TODO: Make this a union, if possible! ( First attempt was a fail ;) )
		{
			sf::FloatRect* collidersPointer;
			sf::FloatRect collidersValue;
		} colliders;
	};
public:
	class Body
	{
		friend class Physics;

		bool isStatic;
		bool isTrigger;
		bool isGrounded = false;
		bool triggered = false;
		sf::Vector2f pos;
		PhysicElement physicsElement;
		//TODO: Add information what was hit (id) if isGrounded... you know
	public:
		sf::Vector2f vel = { 0.0f, 0.0f };
	private:
		void updateShoes();
		void updateLeftRightBody();
	public:
		//Should be called, if the object is moving
		Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, bool isTrigger = false, bool isStatic = false, std::vector<std::string> collisionId = {});
		//Should be called if the object, is a static one
		Body(std::string name, sf::FloatRect collider, bool isTrigger = false, bool isStatic = true, std::vector<std::string> collisionId = {});
		bool getIsGrounded();
		bool getIsTriggerd();
		sf::Vector2f& getPos();
	};
private:
	static constexpr float gravity = 9.81f;
	std::unordered_map<std::string, std::shared_ptr<Body>> bodies;
public:
	Physics();
	void update(float dt);
	void addElementPointer(std::shared_ptr<Body> body);
	void addElementValue(Body body);
};