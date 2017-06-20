#include "ChestComponent.h"

void ChestComponent::eventCollectedAllDiamondsHandler(std::unique_ptr<EventData> eventData)
{
	behaviour = BEHAVIOUR::OPENING;
}

void ChestComponent::shake(int& i)
{
	if (counter <= 5)
		sprite.setRotation(-counter);
	else if (counter > 5 && counter <= 10)
		sprite.setRotation(counter - 10);
	else if (counter > 10 && counter <= 15)
		sprite.setRotation(counter - 10);
	else if (counter > 15)
	{
		sprite.setRotation(counter - (14 + i));
		i += 2;
	}

	++counter;
}

ChestComponent::ChestComponent(sf::FloatRect& boundingBox, std::shared_ptr<sf::Texture> texture, sf::RenderWindow * renderTarget, Physics* physics, EventManager* eventManager, Actor* owner) : pos(pos), texture(texture),
renderTarget(renderTarget), sprite(*texture), boundingBox(boundingBox),
body(std::make_shared<Physics::Body>(std::string("chest"), this->boundingBox, true, true, std::vector<std::string>{"player"})),
physics(physics), Component(COMPONENT_CHEST_ID, eventManager, owner)
{
	eventManager->addListener(EventCollectedAllDiamonds::EVENT_COLLECTEDALLDIAMONDS_ID, delegateCollectedAllDiamonds);

	sprite.setOrigin(offset);
	sprite.setPosition(boundingBox.left + offset.x, boundingBox.top + offset.y);
	physics->addElementPointer(body);
}

void ChestComponent::update(float dt)
{
	if (counter <= 20)
	{
		if (body)
		{
			if (body->getIsTriggerd() && body->getTriggerInformation().triggerElementCollision == "player")
			{
				if (behaviour == BEHAVIOUR::OPENING)
				{
					physics->removeElementById(body->getId());
					counter = 0;
					sprite.setRotation(0.0f);
					if (body.unique())
						body.reset();
				}
				else
				{
					if (counter >= 19)
					{
						counter = 0;
						i = 0;
					}

					shake(i);
				}
			}
			else if (counter < 19)
			{
				shake(i);
			}
		}
		else if (counter < 20)
		{
			sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left, sprite.getTextureRect().top + 1, sprite.getTextureRect().width, sprite.getTextureRect().height - 1));
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + 1.0f);
			counter++;
		}
		else if (counter == 20)
		{
			if (behaviour == BEHAVIOUR::OPENING)
			{
				if (eventManager->TriggerEvent(std::make_unique<EventAddKey>()))
				{
					++counter;
					return;
				}
				else
					std::cout << "ChestComponent::Update eventAddKey not processed!" << std::endl;
			}
		}
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
