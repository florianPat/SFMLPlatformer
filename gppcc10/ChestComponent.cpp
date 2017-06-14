#include "ChestComponent.h"

ChestComponent::ChestComponent(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow * renderTarget, Physics* physics) : pos(pos), texture(texture),
renderTarget(renderTarget), sprite(*texture), boundingBox(boundingBox),
body(std::make_shared<Physics::Body>(std::string("chest"), this->boundingBox, true, true, std::vector<std::string>{"player"})),
physics(physics), Component(COMPONENT_CHEST)
{
	sprite.setPosition(boundingBox.left, boundingBox.top);
	physics->addElementPointer(body);
}

void ChestComponent::update(float dt)
{
	if (body)
	{
		if (body->getIsTriggerd() && body->getTriggerInformation().triggerElementCollision == "player")
		{
			physics->removeElementById(body->getId());
			if (body.unique())
				body.reset();
		}
	}
	else if (counter <= 20)
	{
		sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left, sprite.getTextureRect().top + 1, sprite.getTextureRect().width, sprite.getTextureRect().height - 1));
		sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + 1.0f);
		counter++;
	}
	else
	{
		//TODO: Spawn key
	}
}

void ChestComponent::draw()
{
	renderTarget->draw(sprite);
}

std::shared_ptr<Physics::Body> ChestComponent::getBody()
{
	return body;
}
