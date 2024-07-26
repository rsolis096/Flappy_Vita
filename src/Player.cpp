#include "Player.h"

#include <stdexcept>

const float gravity = 0.45;

Player::Player(const char *fN, float x, float y)
{
	// Position Related
	actorPosition.x = Game::SCREEN_X * 0.05;
	actorPosition.y = Game::SCREEN_Y / 2;
	actorHeight = x;
	actorWidth = y;
	playerSprite = {static_cast<int>(actorPosition.x), static_cast<int>(actorPosition.y), static_cast<int>(actorWidth), static_cast<int>(actorHeight)};
	hitBox = {static_cast<int>(actorPosition.x), static_cast<int>(actorPosition.y), static_cast<int>(actorWidth) - 25, static_cast<int>(actorHeight) - 25};
	rotation = 0.0;

	// Movement Related
	hoverSpeed = 0.1;
	fallVelocity = 0.0f;

	// Texture Related
	fileName = fN;
	sprite[1] = nullptr;
	sprite[0] = nullptr;
	spriteSurface[1] = nullptr;
	spriteSurface[0] = nullptr;

	// Bool
	isAlive = true;
	isFalling = false;
	allowRotation = true;
}

void Player::Jump()
{
	isFalling = true;
	rotation -= 25; // Used for the climbing effect when flapping
	if (fallVelocity > 0)
		fallVelocity = -30.0f;
	else
		fallVelocity += -40.0f;
}

// update positon of player
void Player::UpdateActor(float deltaTime)
{

	// Gravity
	if (isFalling == true)
	{
		// ACTOR POSITION 0.0f = top of screen, increase this value to "fall".

		// Update position in terms of meters
		actorPosition.y += fallVelocity * (Game::deltaTime) * 10.0f;

		if (actorPosition.y > Game::SCREEN_Y * 0.88)
			actorPosition.y = Game::SCREEN_Y * 0.88;

		// Adjust fallVelocity based on impact of gravity
		fallVelocity += 9.81f * Game::deltaTime * 10.0f;

		if (rotation < 0)
		{
			// Used for the climbing effect when flapping
			rotation += 1;
			if (rotation > 0)
				rotation = 0;
		}
	}

	// Prevent player from falling through the floor
	if (static_cast<int>(actorPosition.y + actorHeight / 2) >= 956)
	{
		actorPosition.y = Game::SCREEN_Y - actorHeight / 2;
		isFalling = false;
		allowRotation = false;
	}
	// Prevent player from going above play area
	if (static_cast<int>(actorPosition.y) <= 0)
	{
		actorPosition.y = actorHeight / 2;
		isFalling = true;
	}
}

void Player::drawActor(SDL_Renderer *mRenderer, int index)
{
	// Player flap animation
	if (index == 0 && isAlive == true)
		hoverSpeed = 0.5;
	else if (index == 1 && isAlive == true)
		hoverSpeed = -0.5;
	else
	{
		hoverSpeed = 0.0f; // This prevents the player from rising when they die
		index = 0;		   // This stops the wings from flapping after death
	}

	// Updates the players position (it's sprite)
	playerSprite.x = static_cast<int>(actorPosition.x);
	playerSprite.y = static_cast<int>(actorPosition.y - actorHeight / 2);

	// Collision hitbox
	hitBox.x = static_cast<int>(actorPosition.x + actorWidth / 4);
	hitBox.y = static_cast<int>(actorPosition.y - actorHeight / 2.5);

	// Show hitbox for debugging
	// SDL_RenderDrawRect(mRenderer, &hitBox);
	SDL_RenderCopyEx(mRenderer, sprite[index], nullptr, &playerSprite, rotation, nullptr, SDL_FLIP_NONE);
}

void Player::loadTexture(SDL_Renderer *mRenderer)
{
	SDL_DestroyTexture(sprite[0]); // Prevents memory leak
	SDL_DestroyTexture(sprite[1]); // Prevents memory leak
	spriteSurface[0] = IMG_Load("Assets/textures/frame-1.png");
	spriteSurface[1] = IMG_Load("Assets/textures/frame-2.png");
	if (spriteSurface[0] == nullptr || spriteSurface[1] == nullptr)
		SDL_Log("Texture failed to load: %s", SDL_GetError());
	else
	{
		sprite[0] = SDL_CreateTextureFromSurface(mRenderer, spriteSurface[0]);
		sprite[1] = SDL_CreateTextureFromSurface(mRenderer, spriteSurface[1]);
		if (spriteSurface[0] == nullptr || spriteSurface[1] == nullptr)
			throw std::runtime_error(SDL_GetError());
	}
	SDL_FreeSurface(spriteSurface[0]);
	SDL_FreeSurface(spriteSurface[1]);
}

Player::~Player()
{
	for (int i = 0; i < 2; i++)
	{
		SDL_DestroyTexture(sprite[i]);
		sprite[i] = nullptr;
		spriteSurface[i] = nullptr;
	}
}

void Player::flap()
{
	actorPosition.y += hoverSpeed;
}

int Player::getWidth()
{
	return actorWidth;
}

int Player::getHeight()
{
	return actorHeight;
}

// Returns the current x position of the actor
int Player::getPositionX()
{
	return static_cast<int>(actorPosition.x);
}

// Returns the current y position of the actor
int Player::getPositionY()
{
	return playerSprite.y;
}

// Returns the hitbox for collision detection
SDL_Rect Player::getHitBox()
{
	return hitBox;
}

void Player::restartGame()
{
	actorPosition.x = Game::SCREEN_X * 0.05;
	actorPosition.y = Game::SCREEN_Y / 2;
	rotation = 0;
	isAlive = true;
	isFalling = false;
	allowRotation = true;
}