#include "Level.h"

void Level::eventLevelReloadHandler(std::unique_ptr<EventData>)
{
	newLevel = levelName;
	endLevel = true;
}

void Level::updateModel()
{
	float dt = clock.restart().asSeconds();

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.drawActors();

	window->display();
}

void Level::createDiamonds(std::vector<sf::FloatRect>& objectGroup)
{
	Actor diamond(ACOTOR_DIAMOND);
	Actor* diamondP = gom.addActor(diamond);

	std::shared_ptr<DiamondCompoennt> diamondComponent = std::make_shared<DiamondCompoennt>(objectGroup, window, &physics, &eventManager, diamondP);
	diamondP->addComponent(diamondComponent);
}

void Level::createChest()
{
	Actor chest(ACTOR_CHEST);
	Actor* chestP = gom.addActor(chest);

	std::shared_ptr<ChestComponent> chestComponent = std::make_shared<ChestComponent>(map.getObjectGroup("truhe")[0], Assets::textureAssetManager.getOrAddRes("assetsRaw/64x64/Truhe.png"), window, &physics, &eventManager, chestP);
	chestP->addComponent(chestComponent);
}

void Level::createPlayer(sf::Vector2f& playerPos)
{
	Actor player(ACTOR_PLAYER);
	Actor* playerP = gom.addActor(player);

	std::shared_ptr<PlayerComponent> playerComponent = std::make_shared<PlayerComponent>(playerPos, TextureAtlas("player.atlas"), window, &eventManager, playerP);
	playerP->addComponent(playerComponent);
	physics.addElementPointer(playerComponent->getBody());
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager(),
eventLevelReloadFunction(std::bind(&Level::eventLevelReloadHandler, this, std::placeholders::_1)), delegateLevelReload(std::make_pair(delegateLevelReloadId, eventLevelReloadFunction))
{
	sf::Vector2f playerPos = { 0.0f, 0.0f };

	auto mapObjectGroups = map.getObjectGroups();
	for (auto it = mapObjectGroups.begin(); it != mapObjectGroups.end(); ++it)
	{
		if (it->name == "Ground")
		{
			for (size_t i = 0; i < map.getObjectGroup("Ground").size(); ++i)
				physics.addElementValue(Physics::Body(std::string("ground" + std::to_string(i)), map.getObjectGroup("Ground")[i]));
		}
		else if (it->name == "truhe")
		{
			createChest();
		}
		else if (it->name == "playerStart")
		{
			playerPos.x = it->objects[0].left;
			playerPos.y = it->objects[0].top;
		}
		else if (it->name == "diamonds")
		{
			createDiamonds(it->objects);
		}
	}

	createPlayer(playerPos);

	eventManager.addListener(EventLevelReload::EVENT_LEVEL_RELOAD_ID, delegateLevelReload);
}

std::string Level::Go()
{
	while (!endLevel && window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		updateModel();
		composeFrame();
	}

	return newLevel;
}
