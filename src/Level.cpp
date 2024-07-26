#include "Level.h"
#include <psp2/kernel/rng.h>

// Returns a true random value
int Level::getRandom()
{
    int max = static_cast<int>(Game::SCREEN_Y * 0.20f);
    int min = static_cast<int>(-Game::SCREEN_Y * 0.20f);

    int randomValue;
    sceKernelGetRandomNumber(&randomValue, sizeof(randomValue));

    // Convert randomValue to an unsigned value for modulo operation
    unsigned int range = static_cast<unsigned int>(max - min + 1);
    unsigned int unsignedRandomValue = static_cast<unsigned int>(randomValue);

    // Get a random value in the range [0, range-1]
    unsigned int rangeValue = unsignedRandomValue % range;

    // Convert rangeValue to the desired range [min, max]
    int result = min + static_cast<int>(rangeValue);

    return result;
}

Level::Level(SDL_Renderer *mRenderer)
{
    // Set of objects		{x,y,w,h}
    topObjects.push_back({300, -static_cast<int>(Game::SCREEN_Y * 0.55), static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)});  // Top
    bottomObjects.push_back({300, static_cast<int>(Game::SCREEN_Y * 0.6), static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)}); // Bottom

    int offset2 = getRandom();
    topObjects.push_back({600, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset2, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)});  // Top
    bottomObjects.push_back({600, static_cast<int>(Game::SCREEN_Y * 0.6) + offset2, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)}); // Bottom

    int offset3 = getRandom();
    topObjects.push_back({900, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset3, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)});  // Top
    bottomObjects.push_back({900, static_cast<int>(Game::SCREEN_Y * 0.6) + offset3, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)}); // Bottom

    int offset4 = getRandom();
    topObjects.push_back({1200, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset4, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)});  // Top
    bottomObjects.push_back({1200, static_cast<int>(Game::SCREEN_Y * 0.6) + offset4, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)}); // Bottom

    pointObject.push_back({0, 0, 5, 10000});
    pointObject.push_back({-100, 0, 5, 10000});
    pointObject.push_back({-100, 0, 5, 10000});
    pointObject.push_back({-100, 0, 5, 10000});

    topObjectSurface = IMG_Load("Assets/textures/pipeTop.png");
    topObjectTexture = SDL_CreateTextureFromSurface(mRenderer, topObjectSurface);
    SDL_FreeSurface(topObjectSurface);

    bottomObjectSurface = IMG_Load("Assets/textures/pipeBottom.png");
    bottomObjectTexture = SDL_CreateTextureFromSurface(mRenderer, bottomObjectSurface);
    SDL_FreeSurface(bottomObjectSurface);
}

void Level::drawLevel(SDL_Renderer *mRenderer)
{

    //  Render pipes with opening facing downwards, moves them based on random diff
    for (int i = 0; i < topObjects.size(); i++)
    {
        // Handle top objects
        SDL_Rect *topElement = &topObjects[i];
        SDL_Rect *bottomElement = &bottomObjects[i];
        if (topElement->x > -topElement->w && bottomElement->x > -bottomElement->w)
        {
            SDL_RenderCopy(mRenderer, topObjectTexture, nullptr, topElement);
            SDL_RenderCopy(mRenderer, bottomObjectTexture, nullptr, bottomElement);
        }
        else
        {
            int diff = getRandom();
            topElement->x += 1200;
            topElement->y = -static_cast<int>(Game::SCREEN_Y * 0.55);
            topElement->y += diff;

            bottomElement->x += 1200;
            bottomElement->y = static_cast<int>(Game::SCREEN_Y * 0.6);
            bottomElement->y += diff;
        }
    }

    // Move point markers, not rendered
    pointObject[0].x = topObjects[0].x + topObjects[0].w / 2;
    pointObject[1].x = topObjects[1].x + topObjects[0].w / 2;
    pointObject[2].x = topObjects[2].x + topObjects[1].w / 2;
    pointObject[3].x = topObjects[3].x + topObjects[2].w / 2;

    // Draw point objects
    // SDL_RenderDrawRect(mRenderer, &pointObject[0]);
    // SDL_RenderDrawRect(mRenderer, &pointObject[1]);
    // SDL_RenderDrawRect(mRenderer, &pointObject[2]);
    // SDL_RenderDrawRect(mRenderer, &pointObject[3]);
}

void Level::updatePosition(int offset)
{

    for (SDL_Rect &object : topObjects)
        object.x -= offset;
    for (SDL_Rect &object : bottomObjects)
        object.x -= offset;
}

Level::~Level()
{
    SDL_DestroyTexture(topObjectTexture);
    SDL_DestroyTexture(bottomObjectTexture);
    // SDL_FreeSurface(topObjectSurface);
    // SDL_FreeSurface(bottomObjectSurface);

    topObjectTexture = nullptr;
    topObjectSurface = nullptr;
    bottomObjectTexture = nullptr;
    bottomObjectSurface = nullptr;
}

void Level::restartGame()
{
    // Resets positions back to start

    topObjects[0] = {300, -static_cast<int>(Game::SCREEN_Y * 0.55), static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};
    bottomObjects[0] = {300, static_cast<int>(Game::SCREEN_Y * 0.6), static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};

    int offset2 = getRandom();
    topObjects[1] = {600, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset2, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};
    bottomObjects[1] = {600, static_cast<int>(Game::SCREEN_Y * 0.6) + offset2, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};

    int offset3 = getRandom();
    topObjects[2] = {900, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset3, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};
    bottomObjects[2] = {900, static_cast<int>(Game::SCREEN_Y * 0.6) + offset3, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};

    int offset4 = getRandom();
    topObjects[3] = {1200, -static_cast<int>(Game::SCREEN_Y * 0.55) + offset4, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};
    bottomObjects[3] = {1200, static_cast<int>(Game::SCREEN_Y * 0.6) + offset4, static_cast<int>(Game::SCREEN_X * 0.08), static_cast<int>(Game::SCREEN_Y * 0.9375)};

    pointObject[0] = {0, 0, 5, 10000};
    pointObject[1] = {-100, 0, 5, 10000};
    pointObject[2] = {-100, 0, 5, 10000};
    pointObject[3] = {-100, 0, 5, 10000};
}