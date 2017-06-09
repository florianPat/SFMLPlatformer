#pragma once

#include "Animation.h"
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Physics.h"

class Chest
{
	sf::Vector2f pos;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	sf::RenderWindow* renderTarget;
	Physics* physics;
	sf::FloatRect boundingBox;
	std::shared_ptr<Physics::Body> body;
public:
	Chest(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow* renderTarget, Physics* physics);
	void update(float dt);
	void draw();
	std::shared_ptr<Physics::Body> getBody();
};