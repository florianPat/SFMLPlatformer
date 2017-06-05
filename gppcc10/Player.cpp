#include "Player.h"

void Player::addAnimation(std::vector<std::string> regionNames, std::string animationName)
{
	std::vector<TextureRegion> regions;
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		auto region = atlas.findRegion(*it);
		if (region == nullptr)
			continue;
		else
		{
			regions.push_back(*region);
		}
	}

	animations.emplace(animationName, Animation(regions, sf::seconds(0.2f).asMicroseconds(), Animation::PlayMode::LOOPED));
}

Player::Player(sf::Vector2f& pos, TextureAtlas& atlas) : pos(pos), animations(), atlas(atlas), 
														 currentFrame(), shoes(), boundingBox()
{
	addAnimation({ "PlayerIdel" }, "idle");
	addAnimation({ "PlayerWalk1", "PlayerWalk2" }, "walking");
	addAnimation({ "PlayerJump" }, "jump");
}

void Player::update(float dt)
{
	vel.x = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		vel.x = speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		vel.x -= speed;

	pos += vel * dt;

	if (vel.x < 0 || vel.x > 0)
		currentFrame = animations.find("walking")->second.getKeyFrame();
	else
		currentFrame = animations.find("idle")->second.getKeyFrame();

	currentFrame.setPosition(pos);
	
	shoes.left = currentFrame.getPosition().x + 0.1f;
	shoes.top = currentFrame.getPosition().y + currentFrame.getTextureRect().height - 1.0f;
	shoes.width = currentFrame.getTextureRect().width - 0.1f;
	shoes.height = 1.0f;

	boundingBox.left = currentFrame.getPosition().x;
	boundingBox.top = currentFrame.getPosition().y;
	boundingBox.width = (float) currentFrame.getTextureRect().width;
	boundingBox.height = (float) currentFrame.getTextureRect().height;
}

sf::Sprite Player::draw()
{
	return currentFrame;
}

sf::FloatRect* Player::getShoes()
{
	return &shoes;
}

sf::FloatRect* Player::getBoundingBox()
{
	return &boundingBox;
}