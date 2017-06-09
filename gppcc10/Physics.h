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
		sf::FloatRect head;
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

		enum class TriggerBodyPart
		{
			NONE,
			HEAD,
			SHOES,
			LEFT,
			RIGHT
		};

		struct TriggerInformation
		{
			std::string triggerElementCollision = "";
			TriggerBodyPart triggerBodyPart = TriggerBodyPart::NONE;
		};

		bool isStatic;
		bool isTrigger;
		bool isGrounded = false;
		bool triggered = false;
		TriggerInformation triggerInformation = {};
		sf::Vector2f pos;
		PhysicElement physicsElement;
	public:
		sf::Vector2f vel = { 0.0f, 0.0f };
	private:
		void updateShoes();
		void updateLeftRightBody();
		void updateHead();
	public:
		//Should be called, if the object is moving
		Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, bool isTrigger = false, bool isStatic = false, std::vector<std::string> collisionId = {});
		//Should be called if the object, is a static one
		Body(std::string name, sf::FloatRect collider, bool isTrigger = false, bool isStatic = true, std::vector<std::string> collisionId = {});
		bool getIsGrounded();
		bool getIsTriggerd();
		sf::Vector2f& getPos();
		void setPos(sf::Vector2f newPos);
		TriggerInformation& getTriggerInformation(); //TODO: Make this a callback thing and therefore then remove triggered
		std::string& getId();
	};
private:
	static constexpr float gravity = 9.81f;
	std::unordered_map<std::string, std::shared_ptr<Body>> bodies;
private:
	void handleCollision(std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& it,
		std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& collideElement,
		const sf::FloatRect& bodyRect, const sf::FloatRect& elementRect);
public:
	Physics();
	void update(float dt);
	void addElementPointer(std::shared_ptr<Body> body);
	void addElementValue(Body body);
	bool removeElementById(std::string& id);
};