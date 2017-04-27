#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	
	TextureAtlas atlas("player.atlas");

	std::vector<TextureRegion> animationRegions = atlas.getRegions();

	TextureRegion region;
	atlas.addRegion(region); //Will not work, because region is not filled with value!

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}

	return 0;
}