#pragma once

#include "Component.h"
#include "Animation.h"
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Physics.h"
#include "EventAddKey.h"

class ChestComponent : public Component
{
	sf::Vector2f pos;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	sf::RenderWindow* renderTarget;
	Physics* physics;
	sf::FloatRect boundingBox;
	std::shared_ptr<Physics::Body> body;
	int counter = 0;
public:
	static constexpr int COMPONENT_CHEST_ID = 0x52040c86;
public:
	ChestComponent(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow* renderTarget, Physics* physics, EventManager* eventManager, Actor* owner);
	void update(float dt);
	void draw();
	std::shared_ptr<Physics::Body> getBody();
};