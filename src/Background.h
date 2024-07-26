#pragma once
#include "Game.h"

struct Background
{
	Background();
	~Background();
	const char *fileName[2];
	SDL_Rect BG1;
	SDL_Rect BG2;
	SDL_Texture *bgImage[2];
	SDL_Surface *bgSurface[2];
	void drawBackground(SDL_Renderer *);
	void loadBackground(SDL_Renderer *, const int);
	void updatePosition(const int);
	void restartGame();
};
