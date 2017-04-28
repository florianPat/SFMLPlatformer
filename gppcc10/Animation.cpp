#include "Animation.h"
#include <assert.h>

Animation::Animation(std::vector<TextureRegion>& keyFrames, sf::Int64 frameDuration, PlayMode type) : keyFrames(),
	frameDuration(frameDuration), playMode(type), clock(), keyFrameIt(), keyFrameItReverse(), currentKeyFrame()
{
	for (auto it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		this->keyFrames.push_back(it->getRegion());
	}

	keyFrameIt = this->keyFrames.begin();
	keyFrameItReverse = this->keyFrames.rbegin();
	currentKeyFrame = *keyFrameIt;
}

Animation::PlayMode Animation::getPlayMode()
{
	return playMode;
}

sf::Int64 Animation::getFrameDuration()
{
	return frameDuration;
}

void Animation::setFrameDuration(sf::Int64 newFrameDuration)
{
	if (newFrameDuration > 0)
	{
		frameDuration = newFrameDuration;
	}
}

void Animation::setKeyFrames(std::vector<TextureRegion>& newKeyFrames)
{
	keyFrames.clear();
	for (auto it = newKeyFrames.begin(); it != newKeyFrames.end(); ++it)
	{
		keyFrames.push_back(it->getRegion());
	}
}

bool Animation::isAnimationFinished()
{
	assert(playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::REVERSED);

	sf::Time time = clock.restart();
	sf::Int64 currentTime = time.asMicroseconds();
	sf::Int64 framesToAdvance = currentTime / frameDuration;

	if (playMode == Animation::PlayMode::NORMAL)
	{
		for (int i = 0; keyFrameIt != keyFrames.end() || i < framesToAdvance; ++keyFrameIt, ++i)
		{
			currentKeyFrame = *keyFrameIt;
		}
		if (keyFrameIt == keyFrames.end())
			return true;
		else
			return false;
	}
	else if (playMode == Animation::PlayMode::REVERSED)
	{
		for (int i = 0; keyFrameItReverse != keyFrames.rend() || i < framesToAdvance; ++keyFrameIt, ++i)
		{
			currentKeyFrame = *keyFrameIt;
		}
		if (keyFrameItReverse == keyFrames.rend())
			return true;
		else
			return false;
	}
}

sf::Sprite Animation::getKeyFrame()
{
	sf::Time time = clock.restart();
	sf::Int64 currentTime = time.asMicroseconds();
	sf::Int64 framesToAdvance = currentTime / frameDuration;

	if (playMode == Animation::PlayMode::NORMAL)
	{
		for (int i = 0; keyFrameIt != keyFrames.end() || i < framesToAdvance; ++keyFrameIt, ++i)
		{
			currentKeyFrame = *keyFrameIt;
		}
		if (keyFrameIt == keyFrames.end())
		{
			//Handle error better!
			return sf::Sprite();
		}
		else
		{
			return currentKeyFrame;
		}
	}
	else if (playMode == Animation::PlayMode::LOOPED)
	{
		int i = 0;
		while (i < framesToAdvance)
		{
			for (; keyFrameIt != keyFrames.end() || i < framesToAdvance; ++keyFrameIt, ++i)
			{
				currentKeyFrame = *keyFrameIt;
			}
			if (keyFrameIt == keyFrames.end())
			{
				keyFrameIt = keyFrames.begin();
			}
		}

		return currentKeyFrame;
	}
	else if (playMode == Animation::PlayMode::REVERSED)
	{
		for (int i = 0; keyFrameItReverse != keyFrames.rend() || i < framesToAdvance; ++keyFrameIt, ++i)
		{
			currentKeyFrame = *keyFrameIt;
		}
		if (keyFrameItReverse == keyFrames.rend())
		{
			//TODO: Handle error better!
			return sf::Sprite();
		}
		else
		{
			return currentKeyFrame;
		}
	}
	else if (playMode == Animation::PlayMode::LOOP_REVERSED)
	{
		int i = 0;
		while (i < framesToAdvance)
		{
			for (; keyFrameItReverse != keyFrames.rend() || i < framesToAdvance; ++keyFrameIt, ++i)
			{
				currentKeyFrame = *keyFrameIt;
			}
			if (keyFrameItReverse == keyFrames.rend())
			{
				keyFrameItReverse = keyFrames.rbegin();
			}
		}

		return currentKeyFrame;
	}
	else if (playMode == Animation::PlayMode::LOOP_RANDOM)
	{
		int keyFrameSize = keyFrames.size();
		int element = rand() % --keyFrameSize;
		currentKeyFrame = keyFrames[element];

		return currentKeyFrame;
	}
}

void Animation::setPlayMode(PlayMode & newPlayMode)
{
	playMode = newPlayMode;
	keyFrameIt = keyFrames.begin();
	keyFrameItReverse = keyFrames.rbegin();
}

void Animation::restartFrameTimer()
{
	clock.restart();
}
