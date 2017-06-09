#include "Chest.h"

Chest::Chest(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow * renderTarget, Physics* physics) : pos(pos), texture(texture),
	renderTarget(renderTarget), sprite(*texture), boundingBox(boundingBox),
	body(std::make_shared<Physics::Body>(std::string("chest"), this->boundingBox, true, true, std::vector<std::string>{"player"})),
	physics(physics)
{
	sprite.setPosition(boundingBox.left, boundingBox.top);
	physics->addElementPointer(body);
}

void Chest::update(float dt)
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
}

void Chest::draw()
{
	renderTarget->draw(sprite);
}

std::shared_ptr<Physics::Body> Chest::getBody()
{
	return body;
}
