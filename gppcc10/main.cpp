#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "Animation.h"
#include "TiledMap.h"
#include "Assets.h"
#include "Player.h"
#include "Physics.h"
#include <iostream>

PHYSICS_HANDLER(physicsHandler)
{
	std::cout << elementId1  << " " << elementId2<< std::endl;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	/*sf::View view;
	view.zoom(2.0f);
	window.setView(view);*/
	
	TextureAtlas playerAtlas("player.atlas");
	Player player(sf::Vector2f(0.0f, 450.0f), playerAtlas);

	TiledMap map("testLevel.tmx");

	Physics physics(physicsHandler);
	physics.addElementPointer("playerBoundingBox", { player.getBoundingBox() }, { "ground" });
	physics.addElementValue("ground", map.getObjectGroup("Ground") );

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

		player.update(dt);

		physics.update();

		//Render
		window.clear();
		map.draw(window);
		window.draw(player.draw());
		
		window.display();
	}

	return 0;
}