#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "Animation.h"
#include "TiledMap.h"
#include "Assets.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	/*sf::View view;
	view.zoom(2.0f);
	window.setView(view);*/
	
	TextureAtlas atlas("player.atlas");
	Animation anim(atlas.getRegions(), sf::seconds(0.2f).asMicroseconds(), Animation::PlayMode::LOOPED);

	TiledMap map("testLevel.tmx");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		map.draw(window);
		window.draw(anim.getKeyFrame());
		window.display();
	}

	return 0;
}