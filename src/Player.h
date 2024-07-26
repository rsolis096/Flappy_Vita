#pragma once

#include "Game.h"


class Player
{
	public:
		Player(const char*, float, float);
		~Player();//override
		void Jump();
		void UpdateActor(float);
		void drawActor(SDL_Renderer*, int);
		void loadTexture(SDL_Renderer*);
		void flap();
		void restartGame();
		SDL_Rect getHitBox();
		SDL_Rect hitBox;
		int getPositionX();
		int getPositionY();
		int getWidth();
		int getHeight();

		bool isAlive;
		double rotation;
		bool allowRotation;

protected:
		//Position Attributes
		Vector2 actorPosition;

		//Attributes
		float actorHeight;
		float actorWidth;
		const char* fileName;
		bool isFalling;


		//Movement Related
		float fallVelocity;
		float hoverSpeed;
		//X-axis movement speed handled in Game::updateGame class

		//Texture Related
		SDL_Texture* sprite[2];
		SDL_Surface* spriteSurface[2];
	private:
		SDL_Rect playerSprite;

};
