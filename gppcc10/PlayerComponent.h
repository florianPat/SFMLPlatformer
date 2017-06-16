#pragma once

#include "Component.h"
#include "Physics.h"
#include "TextureAtlas.h"
#include "Animation.h"
#include "EventAddKey.h"
#include "KeyComponent.h"

class PlayerComponent : public Component
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
	sf::View view;

	std::function<void(std::unique_ptr<EventData>)> eventAddKeyFunction;
	static constexpr int delegateAddKeyId = 0x7b53b11d;
	DelegateFunction delegateAddKey;
private:
	void addAnimation(std::vector<std::string> regionNames, std::string animationName);
	void eventAddKeyHandler(std::unique_ptr<EventData>);
public:
	static constexpr int COMPONENT_PLAYER_ID = 0x16501a3c;
public:
	PlayerComponent(sf::Vector2f& pos, TextureAtlas altas, sf::RenderWindow* renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt);
	void draw();
	std::shared_ptr<Physics::Body> getBody();
};