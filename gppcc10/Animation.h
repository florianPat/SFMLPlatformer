#pragma once

#include <vector>
#include "TextureAtlas.h"

class Animation
{
public:
	enum class PlayMode
	{
		LOOPED,
		LOOP_REVERSED,
		NORMAL,
		REVERSED
	};
private:
	std::vector<sf::Sprite> keyFrames;
	std::vector<sf::Sprite>::iterator keyFrameIt;
	std::vector<sf::Sprite>::reverse_iterator keyFrameItReverse;
	sf::Int64 currentTime = 0;
	sf::Int64 frameDuration;
	PlayMode playMode;
	int randomElement = 0;
	sf::Clock clock;

	bool paused = false;
public:
	Animation(std::vector<TextureRegion>& keyFrames, sf::Int64 frameDuration, PlayMode type);
	PlayMode getPlayMode();
	sf::Int64 getFrameDuration();
	void setFrameDuration(sf::Int64 frameDuration);
	void setKeyFrames(std::vector<TextureRegion>& keyFrames);
	bool isAnimationFinished();
	sf::Sprite getKeyFrame();
	void setPlayMode(PlayMode& playMode);
	void restartFrameTimer();
	void pause();
	void resume();
	void restart();
};