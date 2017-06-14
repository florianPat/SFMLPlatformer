#include "PlayerComponent.h"

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

PlayerComponent::PlayerComponent(sf::Vector2f& pos, TextureAtlas atlas, sf::RenderWindow* renderTarget) : animations(), atlas(atlas),
currentFrame(), boundingBox(),
body(std::make_shared<Physics::Body>(pos, "player", &boundingBox, false, false, std::vector<std::string>{ "ground", "round", "ound", "und", "nd", "chest" })),
renderTarget(renderTarget), staringPos(pos), view(renderTarget->getDefaultView()), Component(COMPONENT_PLAYER)
{
	addAnimation({ "PlayerIdel" }, "idle");
	addAnimation({ "PlayerWalk1", "PlayerWalk2" }, "walking");
	addAnimation({ "PlayerJump" }, "jump");
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
			body->setPos(sf::Vector2f{ 0.0f, 0.0f });
			body->vel = sf::Vector2f{ 0.0f, 0.0f };
		}
	}

	if (body->getIsTriggerd() && body->getTriggerInformation().triggerElementCollision == "chest")
	{
		/*body->setPos(sf::Vector2f{ 0.0f, 0.0f });
		body->vel = sf::Vector2f{ 0.0f, 0.0f };*/
	}

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