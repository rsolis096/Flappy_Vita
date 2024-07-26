#include "SoundEffect.h"


SoundEffect::SoundEffect(const char *fN, int c, int l, int v)
{
    loops = l;
    channel = c;
    volume = v;
    sE = Mix_LoadWAV(fN);
    if (sE == nullptr)
        SDL_Log("Sound Effect failed to load: %s", SDL_GetError());
}

void SoundEffect::playSound() const
{
    Mix_VolumeChunk(sE, volume);
    Mix_PlayChannel(channel, sE, loops);
}

void SoundEffect::stopSound() const
{
    Mix_Pause(channel);
}

void SoundEffect::resumeSound() const
{
    Mix_Resume(channel);
}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(sE);
    sE = nullptr;
}