#include "TextureAtlas.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "SFML\System\FileInputStream.hpp"
#include "Assets.h"

TextureAtlas::TextureAtlas(const std::string& filepath) : textureAtlas(std::make_unique<std::unordered_map<std::string, TextureRegion>>()),
													fileHeader(std::make_unique<FileHeader>())
{
	std::ifstream file;
	file.open(filepath);

	if (!file)
		std::cerr << "Cant open file!";
	if(!file.is_open())
		std::cerr << "Cant open file!";

	std::string tempString;

	std::getline(file, tempString);

	if (!file.eof())
	{
		for (int i = 0; i < FILE_HEADER_LINE_SIZE; ++i)
		{
			std::string lineContent;
			std::getline(file, lineContent);

			switch (i)
			{
				case 0:
				{
					fileHeader->name = lineContent;
				}	break;
				case 1:
				{
					int width, height;
					std::string widthChar, heightChar;
					
					
					widthChar = getLineContentBetweeen(lineContent, ' ', ',');
					
					size_t nCharsToCopyHeight = lineContent.size();
					heightChar = lineContent.substr(0, nCharsToCopyHeight);

					width = atoi(widthChar.c_str());
					height = atoi(heightChar.c_str());

					fileHeader->size = sf::Vector2i(width, height);
				}	break;
				case 2:
				{
					size_t spacePos = lineContent.find(' ');
					lineContent.erase(0, ++spacePos);
					fileHeader->format = lineContent;
				}	break;
				case 3:
				{
					
					fileHeader->filter[0] = getLineContentBetweeen(lineContent, ' ', ',');
					
					size_t nCharsToCopyFilter1 = lineContent.size();
					fileHeader->filter[1] = lineContent.substr(0, nCharsToCopyFilter1);
				}	break;
				case 4:
				{
					size_t spacePos = lineContent.find(' ');
					lineContent.erase(0, ++spacePos);
					fileHeader->repeat = lineContent;
				}
			}
		}
	}

	while(!file.eof())
	{
		bool isEof = false;
		TextureRegion region;

		region.textureAtlasFileName = fileHeader->name;

		for (int i = 0; i < FILE_LINES_PER_REGION; ++i)
		{
			if (file.eof())
			{
				isEof = true;
				break;
			}

			std::string lineContent;
			std::getline(file, lineContent);

			switch (i)
			{
				case 0:
				{
					region.filename = lineContent;
				}	break;
				case 1: {}	break;
				case 2:
				{
					region.xy = getLineContentRegionValues(lineContent, 'x');
				}	break;
				case 3:
				{
					region.size = getLineContentRegionValues(lineContent, 's');
				}	break;
				case 4: {}	break;
				case 5: {}	break;
				case 6: {}	break;
			}
		}

		if(!isEof)
			textureAtlas->insert({ region.filename, region });
	}

	file.close();

	for (auto i = textureAtlas->begin(); i != textureAtlas->end(); ++i)
	{
		i->second.initSprite();
	}
}

std::unique_ptr<TextureRegion> TextureAtlas::findRegion(const std::string& name)
{
	auto result = textureAtlas->find(name);
	if (result != textureAtlas->end())
	{
		return std::make_unique<TextureRegion>(result->second);
	}
	else
	{
		return nullptr;
	}
}

std::vector<TextureRegion> TextureAtlas::getRegions()
{
	auto result = std::vector<TextureRegion>();
	for (auto it = textureAtlas->begin(); it != textureAtlas->end(); ++it)
	{
		result.push_back(it->second);
	}
	return result;
}

void TextureAtlas::addRegion(const TextureRegion & adder)
{
	if (adder.filename == "" || adder.textureAtlasFileName != fileHeader->name)
		return;
	textureAtlas->insert({ adder.filename, adder });
}

std::string TextureAtlas::getLineContentBetweeen(std::string & lineContent, char first, char secound)
{
	std::string result;

	size_t spacePos = lineContent.find(first);
	lineContent.erase(0, ++spacePos);

	size_t kommaPos = lineContent.find(secound);

	size_t nCharsToCopy = ((int)(lineContent.size() - kommaPos)) - 1;
	result = lineContent.substr(0, nCharsToCopy);

	lineContent.erase(0, ++kommaPos);

	return result;
}

sf::Vector2i TextureAtlas::getLineContentRegionValues(std::string & lineContent, char firstRealChar)
{
	sf::Vector2i result;

	lineContent.erase(0, lineContent.find(firstRealChar));
	result.x = atoi(getLineContentBetweeen(lineContent, ' ', ',').c_str());
	lineContent.erase(0, 1);
	result.y = atoi(lineContent.c_str());

	return result;
}

void TextureRegion::initSprite()
{
	if (textureAtlasFileName == "" || filename == "")
	{
		assert(textureAtlasFileName == "" || filename == "");
		return;
	}
	else
	{
		atlasTexture = Assets::textureAssetManager.getOrAddRes(textureAtlasFileName);
		regionSprite.setTexture(*atlasTexture.get());
		regionSprite.setTextureRect(sf::IntRect(xy.x, xy.y, size.x, size.y));
	}
}

void TextureRegion::setRegion(int x, int y, int widht, int height)
{
	if(x >= 0 && y >= 0)
		if ((unsigned int)widht <= regionSprite.getTexture()->getSize().x && (unsigned int)height <= regionSprite.getTexture()->getSize().y)
		{
			regionSprite.setTextureRect(sf::IntRect(x, y, widht, height));
			xy.x = x;
			xy.y = y;
			size.x = widht;
			size.y = height;
		}
}

sf::Sprite TextureRegion::getRegion()
{
	return regionSprite;
}
