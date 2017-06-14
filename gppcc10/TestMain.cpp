#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "PlayerComponent.h"
#include "ChestComponent.h"

void createPlayer(GameObjectManager& gom, Physics& physics, sf::RenderWindow* window);
void createChest(GameObjectManager& gom, Physics* physics, sf::RenderWindow* window, TiledMap& map);

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

	GameObjectManager gom;
	createChest(gom, &physics, &window, map);
	createPlayer(gom, physics, &window);

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

void createPlayer(GameObjectManager& gom, Physics& physics, sf::RenderWindow* window)
{
	Actor player(0); //TODO: Make id "random"
	std::shared_ptr<PlayerComponent> playerComponent = std::make_shared<PlayerComponent>(sf::Vector2f(0.0f, 0.0f), TextureAtlas("player.atlas"), window);
	player.addComponent(playerComponent);
	physics.addElementPointer(playerComponent->getBody());

	gom.addActor(player);
}

void createChest(GameObjectManager& gom, Physics* physics, sf::RenderWindow* window, TiledMap& map)
{

	Actor chest(1); //TODO: Make id "random"
	std::shared_ptr<ChestComponent> chestComponent = std::make_shared<ChestComponent>(map.getObjectGroup("truhe")[0], Assets::textureAssetManager.getOrAddRes("assetsRaw/64x64/Truhe.png"), window, physics);
	chest.addComponent(chestComponent);

	gom.addActor(chest);
}