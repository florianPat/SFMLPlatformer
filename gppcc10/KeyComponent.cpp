#include "KeyComponent.h"
#include "Actor.h"
#include "PlayerComponent.h"

KeyComponent::KeyComponent(sf::RenderWindow * renderTarget, EventManager* eventManager, Actor* owner) : texture(Assets::textureAssetManager.getOrAddRes("assetsRaw/64x64/Key.png")), 
	sprite(*texture), renderTarget(renderTarget), Component(COMPONENT_KEY_ID, eventManager, owner)
{
	sf::Vector2f playerPos = owner->getComponent<PlayerComponent>(PlayerComponent::COMPONENT_PLAYER_ID)->getBody()->getPos();
	sprite.setPosition(playerPos.x - 60.0f, playerPos.y);
}

void KeyComponent::update(float dt)
{
	PlayerComponent* playerComponent = owner->getComponent<PlayerComponent>(PlayerComponent::COMPONENT_PLAYER_ID);
	sf::Vector2f playerPos = playerComponent->getBody()->getPos();

	if (sprite.getPosition().x + 100.0f < playerPos.x)
		sprite.setPosition(sprite.getPosition().x + speed, sprite.getPosition().y);
	else if (sprite.getPosition().x - 100.0f > playerPos.x)
		sprite.setPosition(sprite.getPosition().x -speed, sprite.getPosition().y);
	
	if (sprite.getPosition().y + 50.0f < playerPos.y)
		sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + speed);
	else if (sprite.getPosition().y - 50.0f > playerPos.y)
		sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - speed);
}

void KeyComponent::draw()
{
	renderTarget->draw(sprite);
}
