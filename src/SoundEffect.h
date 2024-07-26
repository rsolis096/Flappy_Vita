#pragma once

#include "Game.h"

class SoundEffect
{
public:
	SoundEffect(const char *, int, int, int);
	~SoundEffect();
	Mix_Chunk *sE;
	int loops;
	int channel;
	int volume;
	void playSound() const;
	void stopSound() const;
	void resumeSound() const;
};