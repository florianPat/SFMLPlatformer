/*#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "Animation.h"
#include "TiledMap.h"
#include "Assets.h"
#include "Player.h"
#include "Physics.h"
#include <iostream>
#include "Chest.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	window.setFramerateLimit(60);

	sf::View view = window.getDefaultView();
	window.setView(view);

	Physics physics;

	TextureAtlas playerAtlas("player.atlas");
	Player player(sf::Vector2f(0.0f, 0.0f), playerAtlas, &window);
	physics.addElementPointer(player.getBody());

	TiledMap map("testLevel.tmx");
	//TODO: Maybe make this again that you also can pass a vector of sf::FloatRects....
	for (size_t i = 0; i < map.getObjectGroup("Ground").size(); ++i)
		physics.addElementValue(Physics::Body(std::string("ground" + i) /*WHY???*//*, map.getObjectGroup("Ground")[i]));

	Chest chest(map.getObjectGroup("truhe")[0], Assets::textureAssetManager.getOrAddRes("assetsRaw/64x64/Truhe.png"), &window, &physics);

	sf::Clock clock;

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

		//TODO: Make entity-system kind of thing: Handle all update + draw if the object is in vector
		//		of entities, all have pointer to this "handle-class". There they can say fire event with id
		//		wich was defined by constexpr int guid in the first place. Then the approviete handleEvent method 
		//		on the object will be called, like pass it the name, find it in map of entities and go. 
		//		Each entity should also can make a new one or delete one...
		//Is only first test of system, maybe possibly will be greatly modified later on.
		player.update(dt);

		chest.update(dt);

		physics.update(dt);

		view.setCenter({ player.getBody()->getPos().x, view.getCenter().y });
		window.setView(view);

		//Render
		window.clear();
		map.draw(window);
		chest.draw();
		player.draw();
		
		window.display();
	}

	return 0;
}*/