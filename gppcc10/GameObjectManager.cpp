#include "GameObjectManager.h"

GameObjectManager::GameObjectManager() : actors()
{
}

void GameObjectManager::addActor(Actor& actor)
{
	actors.emplace(actor.getId(), std::make_unique<Actor>(actor));
}

void GameObjectManager::destroyActor(int actorId)
{
	auto it = actors.find(actorId);
	if (it != actors.end())
		actors.erase(it);
}

void GameObjectManager::updateActors(float dt)
{
	for (auto it = actors.begin(); it != actors.end(); ++it)
		it->second->update(dt);
}

void GameObjectManager::drawActors()
{
	for (auto it = actors.begin(); it != actors.end(); ++it)
		it->second->draw();
}

Actor* GameObjectManager::getActor(int actorId)
{
	auto it = actors.find(actorId);
	if (it != actors.end())
		return it->second.get();
	else
		return nullptr;
}
