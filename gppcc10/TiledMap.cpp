#include "TiledMap.h"
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "Assets.h"

TiledMap::TiledMap(const std::string & filepath) : tiles(), layers(), objectGroups(), 
												   textureSprite(), texture()
{
	std::ifstream file;
	file.open(filepath);

	if (!file)
		std::cerr << "Cant open file!";
	if (!file.is_open())
		std::cerr << "Cant open file!";

	std::string temp;
	std::getline(file, temp);

	if (!file.eof())
	{
		std::string lineContent;
		std::getline(file, lineContent);


		if (!isWordInLine("orthogonal", lineContent))
			std::cerr << "Map has to be orthogonal!";

		if (!isWordInLine("right-down", lineContent))
			std::cerr << "Maps render-order has to be right-down!";

		mapWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str()); // Maybe make this nicer??
		mapHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

		tileWidth = atoi(getLineContentBetween(lineContent, "tilewidth", '"').c_str());
		tileHeight = atoi(getLineContentBetween(lineContent, "tileheight", '"').c_str());

		//TODO: make functions for these giant code-blocks things...
		//Tile
		std::getline(file, lineContent);

		while (isWordInLine("<tileset", lineContent))
		{
			int firstgrid = atoi(getLineContentBetween(lineContent, "firstgid", '"').c_str());
			int tileCount = atoi(getLineContentBetween(lineContent, "tilecount", '"').c_str());
			for (int i = 0; i < tileCount; ++i)
			{
				std::getline(file, lineContent);
				int id = atoi(getLineContentBetween(lineContent, "id", '"').c_str()) + firstgrid;

				std::getline(file, lineContent);
				int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
				int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());
				std::string source = getLineContentBetween(lineContent, "source", '"');
				tiles.emplace(id, Tile{ id, width, height, Assets::textureAssetManager.getOrAddRes(source) });

				std::getline(file, lineContent); //</tile>
			}
			std::getline(file, lineContent); //</tileset>
			std::getline(file, lineContent); //Maybe new <tileset>...
		}

		//Layer
		while (isWordInLine("<layer", lineContent))
		{
			std::string layerName = getLineContentBetween(lineContent, "name", '"');
			int layerWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
			int layerHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

			layers.emplace(layerName, Layer{ layerName, layerWidth, layerHeight, std::vector<Tile>() });

			auto currentLayer = layers.find(layerName);

			std::getline(file, lineContent); //  <data encoding="csv">
			if (!isWordInLine("csv", lineContent))
				std::cerr << "Maps encoding has to be \"csv\"";

			std::getline(file, lineContent); //Begin of encoding

			for (int y = 0; y < layerHeight; ++y)
			{
				for (int x = 0; x < layerWidth; ++x)
				{
					size_t kommaPos = lineContent.find(',');
					int nextTileId = atoi(lineContent.substr(0, kommaPos).c_str());
					lineContent.erase(0, ++kommaPos);

					Tile nextTile{ 0, 0, 0, nullptr };
					if (tiles.find(nextTileId) != tiles.end())
						nextTile = tiles.find(nextTileId)->second;
					currentLayer->second.tiles.push_back(nextTile);
				}
				std::getline(file, lineContent);
			}
			std::getline(file, lineContent); // </layer>
			std::getline(file, lineContent); //Maybe new <layer>
		}

		//ObjectGroup
		while (isWordInLine("<objectgroup", lineContent))
		{
			std::string objectGroupName = getLineContentBetween(lineContent, "name", '"');
			std::getline(file, lineContent);

			std::vector<sf::IntRect> objectVector;
			while (!isWordInLine("</objectgroup>", lineContent))
			{
				int x = atoi(getLineContentBetween(lineContent, "x", '"').c_str());
				int y = atoi(getLineContentBetween(lineContent, "y", '"').c_str());
				int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
				int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

				objectVector.push_back(sf::IntRect(x, y, width, height));

				std::getline(file, lineContent);
			}
			objectGroups.emplace(objectGroupName, ObjectGroup{ objectGroupName, objectVector });

			std::getline(file, lineContent);
		}

		if (!isWordInLine("</map>", lineContent))
		{
			std::cerr << "Tile Map Constructor: we should be at the end of the file!";
		}

		//Make render texture
		if (texture.create(mapWidth*tileWidth, mapHeight*tileHeight))
		{
			texture.clear();

			for (auto it = layers.begin(); it != layers.end(); ++it)
			{
				for (int y = 0; y < mapHeight; ++y)
				{
					for (int x = 0; x < mapWidth; ++x)
					{
						Layer currentLayer = it->second;
						std::shared_ptr<sf::Texture> source = currentLayer.tiles[mapWidth * y + x].source;
						if (source == nullptr)
							continue;
						sf::Sprite sprite(*source);
						sprite.setPosition((float)x * tileWidth, (float)y * tileHeight);
						texture.draw(sprite);

						texture.display();
					}
				}
			}

			textureSprite = sf::Sprite(texture.getTexture());
		}
		else
			std::cerr << "Could not create Render Texture in TileMap Constructor!";
	}
}

std::vector<sf::IntRect> TiledMap::getObjectGroup(const std::string& objectGroupName)
{
	auto result = objectGroups.find(objectGroupName);
	if (result != objectGroups.end())
		return result->second.objects;
	else
	{
		std::vector<sf::IntRect> result;
		return result; //TODO: Handle this and other cases better, maybe exception????
	}
}

void TiledMap::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(textureSprite);
}

bool TiledMap::isWordInLine(const std::string & word, const std::string & lineContent)
{
	size_t o = 0;
	bool result = false;
	while (o < lineContent.size() && !result)
	{
		o = lineContent.find(word[0], o);
		std::string searchWord(word);
		auto it = ++searchWord.begin();
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

size_t TiledMap::getEndOfWord(const std::string & word, const std::string & lineContent, bool* result)
{
	size_t o = 0;
	*result = false;
	while (o < lineContent.size() && !(*result))
	{
		std::string searchWord(word);
		auto it = searchWord.begin();
		o = lineContent.find(it[0], o);
		++it;
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				*result = true;
				break;
			}
		}
	}

	return ++o;
}

std::string TiledMap::getLineContentBetween(std::string & lineContent, const std::string & endOfFirst, char secound)
{
		std::string result;

		bool resultValue;
		size_t widthEndPos = getEndOfWord(endOfFirst, lineContent, &resultValue);
		if (resultValue)
		{
			lineContent.erase(0, widthEndPos += 2);

			size_t kommaPos = lineContent.find(secound);

			result = lineContent.substr(0, kommaPos);

			lineContent.erase(0, ++kommaPos);
		}

		return result;
}