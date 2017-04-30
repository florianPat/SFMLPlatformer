#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "Animation.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	
	TextureAtlas atlas("player.atlas");

	std::vector<TextureRegion> animationRegions = atlas.getRegions();

	TextureRegion region;
	atlas.addRegion(region); //Will not work, because region is not filled with value!

	sf::Time time = sf::seconds((float)1/4);
	Animation anim(animationRegions, time.asMicroseconds(), Animation::PlayMode::LOOPED);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(anim.getKeyFrame());
		window.display();
	}

	return 0;
}