#pragma once

#include "Component.h"
#include "Animation.h"
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Physics.h"
#include "EventAddKey.h"
#include "EventCollectedAllDiamonds.h"

class ChestComponent : public Component
{
	sf::Vector2f pos;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	sf::RenderWindow* renderTarget;
	Physics* physics;
	sf::FloatRect boundingBox;
	std::shared_ptr<Physics::Body> body;
	int counter = 20;
	int i = 0;

	sf::Vector2f offset = sf::Vector2f(sprite.getTextureRect().width / 2.0f, sprite.getTextureRect().height / 2.0f);

	enum class BEHAVIOUR
	{
		SHAKING,
		OPENING
	} behaviour = BEHAVIOUR::SHAKING;
private:
	void eventCollectedAllDiamondsHandler(std::unique_ptr<EventData> eventData);
private:
	std::function<void(std::unique_ptr<EventData>)> eventCollectedAllDiamondsFunction = std::bind(&ChestComponent::eventCollectedAllDiamondsHandler, this, std::placeholders::_1);
	static constexpr int delegateCollectedAllDiamondsId = 0x9537e02b;
	DelegateFunction delegateCollectedAllDiamonds = std::make_pair(delegateCollectedAllDiamondsId, eventCollectedAllDiamondsFunction);

	void shake(int& i);
public:
	static constexpr int COMPONENT_CHEST_ID = 0x52040c86;
public:
	ChestComponent(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow* renderTarget, Physics* physics, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	std::shared_ptr<Physics::Body> getBody();
};