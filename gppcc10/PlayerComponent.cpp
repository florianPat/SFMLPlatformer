#include "PlayerComponent.h"
#include "Actor.h"
#include "Utils.h"

void PlayerComponent::addAnimation(std::vector<std::string> regionNames, std::string animationName)
{
	std::vector<TextureRegion> regions;
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		auto region = atlas.findRegion(*it);
		if (region == nullptr)
			continue;
		else
		{
			regions.push_back(*region);
		}
	}

	animations.emplace(animationName, Animation(regions, sf::seconds(0.2f).asMicroseconds(), Animation::PlayMode::LOOPED));
}

void PlayerComponent::eventAddKeyHandler(std::unique_ptr<EventData>)
{
	std::shared_ptr<KeyComponent> keyComponent = std::make_shared<KeyComponent>(renderTarget, eventManager, owner);
	owner->addComponent(keyComponent);
}

PlayerComponent::PlayerComponent(sf::Vector2f& pos, TextureAtlas atlas, sf::RenderWindow* renderTarget, EventManager* eventManager, Actor* owner) : animations(), atlas(atlas),
currentFrame(), boundingBox(),
body(std::make_shared<Physics::Body>(pos, "player", &boundingBox, false, false, std::vector<std::string>{ "ground0", "ground1", "ground2", "ground3", "ground4" })),
renderTarget(renderTarget), staringPos(pos), view(renderTarget->getDefaultView()), Component(COMPONENT_PLAYER_ID, eventManager, owner), 
eventAddKeyFunction(std::bind(&PlayerComponent::eventAddKeyHandler, this, std::placeholders::_1)), delegateAddKey(std::make_pair(delegateAddKeyId, eventAddKeyFunction))
{
	addAnimation({ "PlayerIdel" }, "idle");
	addAnimation({ "PlayerWalk1", "PlayerWalk2" }, "walking");
	addAnimation({ "PlayerJump" }, "jump");

	eventManager->addListener(EventAddKey::EVENT_KEYADD_ID, delegateAddKey);
}

void PlayerComponent::update(float dt)
{
	//Walking
	body->vel.x = 0.0f;
	body->vel.y += 9.81f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		body->vel.x = speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		body->vel.x = -speed;

	//jumping
	if (body->getIsGrounded())
		jumpState = JUMP_STATE::GROUNDED;
	else if (jumpState != JUMP_STATE::JUMPING)
		jumpState = JUMP_STATE::FALLING;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && jumpState == JUMP_STATE::GROUNDED)
	{
		jumpState = JUMP_STATE::JUMPING;
		jumpTime = 0;
	}
	if (jumpState == JUMP_STATE::JUMPING)
	{
		jumpTime += sf::seconds(dt).asMicroseconds();
		if (jumpTime > jumpDuration)
		{
			jumpState = JUMP_STATE::FALLING;
		}
		body->vel.y -= jumpSpeed;
	}
	else if (jumpState == JUMP_STATE::GROUNDED)
	{
		body->vel.y = 0;
	}
	else if (jumpState == JUMP_STATE::FALLING)
	{
		if (body->getPos().y > 800.0f)
		{
			std::unique_ptr<EventLevelReload> eventLR = std::make_unique<EventLevelReload>();
			eventManager->TriggerEvent(std::move(eventLR));
		}
	}

	//TODO: Why is he so fast??
	//Diamond Count Upadte
	if (body->getIsTriggerd() && utils::isWordInLine("Diamond", body->getTriggerInformation().triggerElementCollision))
		++diamondsCount;

	//setting...
	if (jumpState != JUMP_STATE::GROUNDED)
		currentFrame = animations.find("jump")->second.getKeyFrame();
	else if (body->vel.x != 0)
		currentFrame = animations.find("walking")->second.getKeyFrame();
	else
		currentFrame = animations.find("idle")->second.getKeyFrame();

	currentFrame.setPosition(body->getPos() + body->vel * dt);

	boundingBox.left = currentFrame.getPosition().x;
	boundingBox.top = currentFrame.getPosition().y;
	boundingBox.width = (float)currentFrame.getTextureRect().width;
	boundingBox.height = (float)currentFrame.getTextureRect().height;
}

void PlayerComponent::draw()
{
	currentFrame.setPosition(body->getPos());
	renderTarget->draw(currentFrame);

	view.setCenter({ body->getPos().x, view.getCenter().y });
	renderTarget->setView(view);
}

std::shared_ptr<Physics::Body> PlayerComponent::getBody()
{
	return body;
}