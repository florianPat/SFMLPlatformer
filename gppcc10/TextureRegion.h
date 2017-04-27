#pragma once

#include <string>
#include "SFML\System\Vector2.hpp"

class TextureRegion
{
public:
	std::string filename;
	sf::Vector2i xy;
	sf::Vector2i size;
public:
	TextureRegion() = default;
	TextureRegion(std::string filename, sf::Vector2i xy, sf::Vector2i size);
};