#include "Background.h"

Background::Background()
{
    fileName[0] = "Assets/textures/bg1.png";
    fileName[1] = "Assets/textures/bg2.png";
    bgImage[0] = nullptr;
    bgSurface[0] = nullptr;
    bgImage[1] = nullptr;
    bgSurface[1] = nullptr;
    BG1 = {0, 0, Game::SCREEN_X, Game::SCREEN_Y};
    BG2 = {Game::SCREEN_X, 0, Game::SCREEN_X, Game::SCREEN_Y};
}

void Background::loadBackground(SDL_Renderer *mRenderer, const int n)
{
    SDL_DestroyTexture(bgImage[n]);
    bgSurface[n] = IMG_Load(fileName[n]);
    if (bgSurface[n] == nullptr)
        SDL_Log("Background failed to load: %s", SDL_GetError());
    else
    {
        bgImage[n] = SDL_CreateTextureFromSurface(mRenderer, bgSurface[n]);
        if (bgImage[n] == nullptr)
            SDL_Log("Background failed to load: %s", SDL_GetError());
    }
    SDL_FreeSurface(bgSurface[n]);
}

void Background::drawBackground(SDL_Renderer *mRenderer)
{

    // When BG1 goes off screen, move it behind BG2
    if (BG1.x <= -Game::SCREEN_X)
    {
        BG1.x = BG2.x + Game::SCREEN_X;
    }
    // When BG2 goes off screen, move it behind BG1
    if (BG2.x <= -Game::SCREEN_X)
    {
        BG2.x = BG1.x + Game::SCREEN_X;
    }

    SDL_RenderCopy(mRenderer, bgImage[0], nullptr, &BG1);
    SDL_RenderCopy(mRenderer, bgImage[1], nullptr, &BG2);
}

void Background::updatePosition(const int offset)
{
    BG1.x -= offset;
    BG2.x -= offset;
}

Background::~Background()
{
    for (int i = 0; i < 2; i++)
    {
        SDL_DestroyTexture(bgImage[i]);
        bgImage[i] = nullptr;
        bgSurface[i] = nullptr;
    }
}

void Background::restartGame()
{
    BG1 = {0, 0, Game::SCREEN_X, Game::SCREEN_Y};
    BG2 = {Game::SCREEN_X, 0, Game::SCREEN_X, Game::SCREEN_Y};
}
