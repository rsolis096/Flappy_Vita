#pragma once
#include "catlog.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "SDL_FontCache.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Text
{
	float x;
	float y;
	std::string text;
	Text(float a, float b, std::string t)
	{
		x = a;
		y = b;
		text = t;
	}
	Text()
	{
		x = 0;
		y = 0;
		text = "";
	}
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void LoadData();
	void UnloadData();
	void restartGame();
	static float deltaTime;
	static int SCREEN_X;
	static int SCREEN_Y;

private:
	void ProcessInput();
	void readTouch();
	void UpdateGame();
	void GenerateOutput();
	void HandlePlayerAnimation();
	void DrawOverlays();
	void DrawText(const std::string &text, float x, float y, int type);
	void HandleDeathSound();
	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool hasScrolled;
	bool deathSoundPlayed;

	std::vector<struct SoundEffect *> mSoundEffects;
	std::vector<struct Text *> mText;

	struct Background *mainBG;
	struct SoundEffect *pointSound, *deathSound, *mainBGMusic, *collideSound, *flapSound;
	struct Text *startTextHeader, *startTextSub, *scoreText, *hScoreText, *restartText;
	class Player *player1;
	struct Level *lvl;
	struct IMG *deathScreen;
	FC_Font *font_small;
	FC_Font *font_large;

	float secondCounter;
	bool startGame;
	int index;
	bool gameStart;
	bool restart;
	bool gameEnd;
	int mSpeed;
	int score;
	int highScore;
};