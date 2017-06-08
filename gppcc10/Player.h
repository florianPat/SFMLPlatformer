#pragma once

#include "Animation.h"
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Physics.h"

class Player
{
	static constexpr float speed = 50.0f;
	static constexpr float jumpSpeed = speed / 2.0f;
	const sf::Int64 jumpDuration = sf::seconds(0.2f).asMicroseconds();
	sf::Int64 jumpTime = 0;
	sf::Vector2f staringPos;
	std::unordered_map<std::string, Animation> animations;
	sf::RenderWindow* renderTarget;
	TextureAtlas atlas;
	sf::Sprite currentFrame;
	sf::FloatRect boundingBox;
	enum class JUMP_STATE
	{
		JUMPING,
		FALLING,
		GROUNDED
	};
	JUMP_STATE jumpState = JUMP_STATE::FALLING;
	std::shared_ptr<Physics::Body> body;
private:
	void addAnimation(std::vector<std::string> regionNames, std::string animationName);
public:
	Player(sf::Vector2f& pos, TextureAtlas& altas, sf::RenderWindow* renderTarget);
	void update(float dt);
	void draw();
	std::shared_ptr<Physics::Body> getBody();
};