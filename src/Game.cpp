#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "Level.h"
#include "Img.h"
#include "SoundEffect.h"

// Vita Includes
#include <psp2/kernel/processmgr.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>

float Game::deltaTime = 0.0f;
float timeSpacePressed = 0.0f;

int Game::SCREEN_Y = 544;
int Game::SCREEN_X = 960;

// Input
SceCtrlData pad;

void Game::DrawText(const std::string &text, float x, float y, int type)
{
	if (type == 0)
		FC_Draw(font_small, mRenderer, x, y, text.c_str());
	else
		FC_Draw(font_large, mRenderer, x, y, text.c_str());
}

Game::Game()
{
	index = 0;
	deltaTime = 0;
	secondCounter = 0;
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;
	mTicksCount = 0;
	player1 = nullptr;
	mainBG = nullptr;
	hasScrolled = false;
	gameStart = false;
	gameEnd = false;
	mSpeed = 0;
	score = 0;
	highScore = 0;
	pointSound = nullptr;
	deathSound = nullptr;
	deathSoundPlayed = false;
	collideSound = nullptr;
	flapSound = 0;
	restart = false;
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Open Audio
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Flappy Bird",			 // Window title
		SDL_WINDOWPOS_UNDEFINED, // Top left x-coordinate of window
		SDL_WINDOWPOS_UNDEFINED, // Top left y-coordinate of window
		SCREEN_X,				 // Width of window
		SCREEN_Y,				 // Height of window
		SDL_WINDOW_SHOWN		 // Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		0);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init())
	{
		SDL_Log("Unable to initialize SDL_font: %s", SDL_GetError());
		return false;
	}

	// Init front touchpad
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);

	LoadData();

	mTicksCount = SDL_GetTicks();
	return true;
}

void Game::LoadData()
{
	player1 = new Player("Assets/textures/frame-1.png", 0.08 * SCREEN_Y, 0.08 * SCREEN_Y);
	player1->loadTexture(mRenderer);

	scoreText = new Text(20, 20, "Score:\t0\n");
	hScoreText = new Text(24, 50, "Best: \t0\n");
	startTextHeader = new Text(SCREEN_X * 0.35, SCREEN_Y * 0.6, "FLAPPY BIRD");
	startTextSub = new Text(SCREEN_X * 0.15, SCREEN_Y * 0.7, "PRESS X or Touch TO START");
	restartText = new Text(SCREEN_X * 0.15, SCREEN_Y * 0.6, "Press X or Touch To Restart");

	font_small = FC_CreateFont();
	font_large = FC_CreateFont();
	FC_LoadFont(font_small, mRenderer, "Assets/font/SuperMario256.ttf", 24, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	FC_LoadFont(font_large, mRenderer, "Assets/font/SuperMario256.ttf", 36, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	mText.emplace_back(scoreText);
	mText.emplace_back(hScoreText);

	mainBG = new Background;
	mainBG->loadBackground(mRenderer, 0);
	mainBG->loadBackground(mRenderer, 1);

	lvl = new Level(mRenderer);
	deathScreen = new IMG("Assets/textures/death.png", 0, SCREEN_Y * 0.35, SCREEN_X, SCREEN_Y * 0.20);
	// Max Volume 128
	pointSound = new SoundEffect("Assets/sounds/point.wav", 1, 0, 35);
	deathSound = new SoundEffect("Assets/sounds/death.wav", 2, 0, 60);
	mainBGMusic = new SoundEffect("Assets/sounds/music.wav", 3, -1, 60);
	collideSound = new SoundEffect("Assets/sounds/collision.wav", 4, 0, 60);
	flapSound = new SoundEffect("Assets/sounds/flap.wav", 5, 0, 128);

	mSoundEffects.push_back(pointSound);
	mSoundEffects.push_back(deathSound);
	mSoundEffects.push_back(mainBGMusic);
	mSoundEffects.push_back(collideSound);
	mSoundEffects.push_back(flapSound);

	mainBGMusic->playSound();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::readTouch()
{
	SceTouchData t;
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &t, 1);
	if (t.reportNum > 0)
	{
		if (!gameEnd)
		{
			timeSpacePressed = SDL_GetTicks();
			gameStart = true;
			mSpeed = 2;
			flapSound->playSound();
			player1->Jump();
		}
		else
		{
			if (gameEnd == true)
				restart = true;
		}
	}
}

void Game::ProcessInput()
{
	sceCtrlPeekBufferPositive(0, &pad, 1);
	if (SDL_GetTicks() - timeSpacePressed > 250)
	{
		readTouch();
		if (pad.buttons & SCE_CTRL_CROSS && !gameEnd)
		{
			timeSpacePressed = SDL_GetTicks();
			gameStart = true;
			mSpeed = 2;
			flapSound->playSound();
			player1->Jump();
		}
		else if (pad.buttons & SCE_CTRL_CROSS && gameEnd)
		{
			if (gameEnd == true)
				restart = true;
		}
		else if (pad.buttons & SCE_CTRL_SELECT)
		{
			mIsRunning = false;
		}
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	// SDL DELAY hurt performance, this is best solution for 60fps lock
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame (converted to seconds)
	deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update Player
	player1->UpdateActor(deltaTime);

	// Detect collision between player pipes
	SDL_Rect hitBox = player1->getHitBox();
	for (int i = 0; i < lvl->topObjects.size(); i++)
	{
		if (SDL_HasIntersection(&lvl->topObjects[i], &hitBox) || SDL_HasIntersection(&lvl->bottomObjects[i], &hitBox))
		{
			player1->isAlive = false;
			gameEnd = true;
			mSpeed = 0;
		}
	}

	// Detect when a player passes in between pipes
	for (auto &element : lvl->pointObject)
	{
		if (player1->getPositionX() >= static_cast<int>(element.x + element.w) && player1->getPositionX() < static_cast<int>(element.x + element.w + 5))
		{
			score++;
			pointSound->playSound();
			scoreText->text = ("Score:\t" + std::to_string(score) + "\n");
		}
	}

	// Kills the player when they hit the ground
	if (player1->getPositionY() >= 0.8 * SCREEN_Y)
	{
		player1->isAlive = false;
		gameEnd = true;
		mSpeed = 0;
	}

	// Scrolling background
	if (mSpeed != 0)
	{
		// Scrolling speed for lvl (pipes) and bg
		lvl->updatePosition(mSpeed + 1.5);
		mainBG->updatePosition(mSpeed + 0.5);
	}
	else
	{
		lvl->updatePosition(0);
		mainBG->updatePosition(0);
	}
	player1->flap(); // Makes the bird hover! NEAT. Uses the draw function

	if (restart == true)
		restartGame();
}

void Game::GenerateOutput()
{
	// Draw Background
	SDL_RenderClear(mRenderer);
	mainBG->drawBackground(mRenderer);

	// Draw Level
	lvl->drawLevel(mRenderer);

	// Handle Player Animation
	HandlePlayerAnimation();

	// Draw Player
	player1->drawActor(mRenderer, index);

	// Draw Overlays (Death Screen, Start Menu)
	DrawOverlays();

	// Draw Score and Text
	for (const auto &element : mText)
	{
		DrawText(element->text, element->x, element->y, 0);
	}

	// Play Death Sound if Necessary (stops other sounds)
	HandleDeathSound();

	// Swap Buffers
	SDL_RenderPresent(mRenderer);
}

void Game::HandlePlayerAnimation()
{
	secondCounter += deltaTime;
	if (secondCounter >= 0.25)
	{
		secondCounter = 0.0f;
		index = (index == 0) ? 1 : 0;
	}
	if (gameEnd && secondCounter >= 0.02 && player1->allowRotation)
	{
		player1->rotation += 1.5;
	}
}

void Game::DrawOverlays()
{
	if (gameEnd)
	{
		deathScreen->drawFadeIn(mRenderer, deltaTime);
		// Show the restart game text
		DrawText(restartText->text, restartText->x, restartText->y, 1);
	}
	else if (!gameStart)
	{
		// If the game hasn't started yet, draw the title and instructions
		DrawText(startTextHeader->text, startTextHeader->x, startTextHeader->y, 1);
		DrawText(startTextSub->text, startTextSub->x, startTextSub->y, 1);
	}
}

void Game::HandleDeathSound()
{
	if (!deathSoundPlayed && gameEnd)
	{
		deathSound->playSound();
		collideSound->playSound();
		deathSoundPlayed = true;
		mainBGMusic->stopSound();
	}
}

void Game::Shutdown()
{
	UnloadData();
	FC_FreeFont(font_small);
	FC_FreeFont(font_large);
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::UnloadData()
{
	for (auto &t : mText)
		t = nullptr;

	startTextHeader = nullptr;
	startTextSub = nullptr;
	restartText = nullptr;
	for (auto &sounds : mSoundEffects)
		delete sounds;
	delete player1;
	delete mainBG;
	delete deathScreen;
	delete lvl;
}

void Game::restartGame()
{
	gameStart = false;
	gameEnd = false;
	restart = false;
	deathSoundPlayed = false;
	deathScreen->setAlpha(0);
	mSpeed = 0;
	if (score > highScore)
	{
		highScore = score;
		hScoreText->text = ("Best: \t" + std::to_string(score) + "\n");
	}
	score = 0;
	player1->restartGame();
	mainBG->restartGame();
	lvl->restartGame();
	mainBGMusic->resumeSound();
}