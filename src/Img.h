#pragma once
#include "Game.h"

struct IMG
{
	IMG(const char *, int, int, int, int);
	~IMG();
	const char *fileName;
	SDL_Rect banner;
	SDL_Texture *bannerTexture;
	SDL_Surface *bannerSurface;
	void drawFadeIn(SDL_Renderer *, float);
	void setAlpha(int);
	int alpha;
	float secondCounter;
};