#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "PlayerComponent.h"
#include "ChestComponent.h"
#include "EventManager.h"
#include <iostream>

constexpr int ACTOR_PLAYER = 0xba7bac8c;
constexpr int ACTOR_CHEST = 0x9035afc9;

void createPlayer(GameObjectManager& gom, Physics& physics, sf::RenderWindow* window, EventManager* eventManager);
void createChest(GameObjectManager& gom, Physics* physics, sf::RenderWindow* window, TiledMap& map, EventManager* eventManager);

void Function(EventData& eventData);

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	window.setFramerateLimit(60);

	Physics physics;

	TiledMap map("testLevel.tmx");
	//TODO: Maybe make this again that you also can pass a vector of sf::FloatRects....
	for (size_t i = 0; i < map.getObjectGroup("Ground").size(); ++i)
		physics.addElementValue(Physics::Body(std::string("ground" + i) /*WHY???*/, map.getObjectGroup("Ground")[i]));

	sf::Clock clock;

	EventManager eventManager;
	
	GameObjectManager gom;
	createChest(gom, &physics, &window, map, &eventManager);
	createPlayer(gom, physics, &window, &eventManager);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Update
		float dt = clock.restart().asSeconds();

		gom.updateActors(dt);

		physics.update(dt);

		//Render
		window.clear();

		map.draw(window);
		gom.drawActors();

		window.display();
	}

	return 0;
}

void createPlayer(GameObjectManager& gom, Physics& physics, sf::RenderWindow* window, EventManager* eventManager)
{
	Actor player(ACTOR_PLAYER);
	Actor* playerP = gom.addActor(player);

	std::shared_ptr<PlayerComponent> playerComponent = std::make_shared<PlayerComponent>(sf::Vector2f(0.0f, 0.0f), TextureAtlas("player.atlas"), window, eventManager, playerP);
	playerP->addComponent(playerComponent);
	physics.addElementPointer(playerComponent->getBody());
}

void createChest(GameObjectManager& gom, Physics* physics, sf::RenderWindow* window, TiledMap& map, EventManager* eventManager)
{
	Actor chest(ACTOR_CHEST);
	Actor* chestP = gom.addActor(chest);

	std::shared_ptr<ChestComponent> chestComponent = std::make_shared<ChestComponent>(map.getObjectGroup("truhe")[0], Assets::textureAssetManager.getOrAddRes("assetsRaw/64x64/Truhe.png"), window, physics, eventManager, chestP);
	chestP->addComponent(chestComponent);
}

void Function(EventData & eventData)
{
	std::cout << "Function functions!!!!!" << std::endl;
}
