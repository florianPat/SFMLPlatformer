#pragma once

#include "Component.h"
#include "SFML\Graphics.hpp"
#include "Assets.h"

class KeyComponent : public Component
{
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	sf::RenderWindow* renderTarget;

	static constexpr float speed = 5.0f;
public:
	static constexpr int COMPONENT_KEY_ID = 0xd3f95aea;
public:
	KeyComponent(sf::RenderWindow* renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};