#pragma once

#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "PlayerComponent.h"
#include "ChestComponent.h"
#include "EventManager.h"
#include "DiamondComponent.h"

class Level
{
private:
	sf::RenderWindow* window;
	Physics physics;
	TiledMap map;
	sf::Clock clock;
	EventManager eventManager;
	GameObjectManager gom;

	std::string levelName;

	std::string newLevel = "";
	bool endLevel = false;


	static constexpr int ACTOR_PLAYER = 0xba7bac8c;
	static constexpr int ACTOR_CHEST = 0x9035afc9;
	static constexpr int ACOTOR_DIAMOND = 0x63f40cbe;

	std::function<void(std::unique_ptr<EventData>)> eventLevelReloadFunction;
	static constexpr int delegateLevelReloadId = 0;
	DelegateFunction delegateLevelReload;
private:
	void eventLevelReloadHandler(std::unique_ptr<EventData>);
private:
	virtual void updateModel();
	virtual void composeFrame();
private:
	void createDiamonds(std::vector<sf::FloatRect>& objectGroup);
	void createChest();
	void createPlayer(sf::Vector2f& playerPos);
public:
	Level(sf::RenderWindow* window, std::string tiledMapName);
	std::string Go();
};