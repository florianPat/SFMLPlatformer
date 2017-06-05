#pragma once

#include "Animation.h"
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Physics.h"

class Player
{
	sf::Vector2f pos, vel = { 0.0f, 0.0f };
	static constexpr float speed = 100.0f;
	const sf::Int64 jumpDuration = sf::seconds(0.2f).asMicroseconds();
	std::unordered_map<std::string, Animation> animations;
	//sf::RenderWindow renderTarget;
	TextureAtlas atlas;
	sf::Sprite currentFrame;
	sf::FloatRect shoes;
	sf::FloatRect boundingBox;
	enum class JUMP_STATE
	{
		FALLING,
		JUMPING,
		GROUNDED
	};
	enum class WALK_STATE
	{
		IDLE,
		LEFT,
		RIGHT
	};
private:
	void addAnimation(std::vector<std::string> regionNames, std::string animationName);
public:
	Player(sf::Vector2f& pos, TextureAtlas& altas);
	void update(float dt);
	sf::Sprite draw();
	sf::FloatRect* getShoes();
	sf::FloatRect* getBoundingBox();
};