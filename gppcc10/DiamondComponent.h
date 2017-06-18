#pragma once

#include "Component.h"
#include "Physics.h"
#include "Assets.h"

class DiamondCompoennt : public Component
{
	sf::RenderWindow* renderTarget;
	std::shared_ptr<sf::Texture> texture;
	std::vector<sf::Sprite> sprites;
	Physics* physics;
	sf::FloatRect boundingBox;
	std::vector<std::shared_ptr<Physics::Body>> bodies;
public:
	static constexpr int COMPONENT_DIAMOND_ID = 0x327633a4;
public:
	DiamondCompoennt(std::vector<sf::FloatRect>& objectGroup, sf::RenderWindow* renderTarget, Physics* physics, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};