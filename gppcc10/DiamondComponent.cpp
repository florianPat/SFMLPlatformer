#include "DiamondComponent.h"

DiamondCompoennt::DiamondCompoennt(std::vector<sf::FloatRect>& objectGroup, sf::RenderWindow * renderTarget, Physics* physics, EventManager * eventManager, Actor * owner) : renderTarget(renderTarget),
physics(physics), texture(Assets::textureAssetManager.getOrAddRes("assetsRaw/nicht64x64/Diamond.png")),
Component(COMPONENT_DIAMOND_ID, eventManager, owner)
{
	int i = 0;
	for (auto it = objectGroup.begin(); it != objectGroup.end(); ++it, ++i)
	{
		sprites.push_back(sf::Sprite(*texture));
		sprites[i].setPosition(it->left, it->top);

		bodies.push_back(std::make_shared<Physics::Body>("Diamond" + i, *it, true, true, std::vector<std::string>{"player"}));
		physics->addElementPointer(bodies[i]);
	}
}

void DiamondCompoennt::update(float dt)
{
	auto itSprites = sprites.begin();
	for (auto it = bodies.begin(); it != bodies.end(); ++it, ++itSprites)
	{
		if (*it)
		{
			if ((*it)->getIsTriggerd() && (*it)->getTriggerInformation().triggerElementCollision == "player")
			{
				physics->removeElementById((*it)->getId());
				if (it->unique())
				{
					auto copySprite = itSprites++;
					itSprites = sprites.erase(copySprite);
					auto copy = it++;
					copy->reset();
					it = bodies.erase(copy);

					if (bodies.size() == 0)
						break;

					if (it != bodies.begin())
						--it;
					if (itSprites != sprites.begin())
						--itSprites;
				}
			}
		}
	}
}

void DiamondCompoennt::draw()
{
	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		renderTarget->draw(*it);
	}
}
