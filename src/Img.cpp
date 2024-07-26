
#include "Img.h"
/*
const char* fN: file directory
int x,y,w,z
*/
IMG::IMG(const char *fN, int x, int y, int w, int h)
{
    fileName = fN;
    banner = {x, y, w, h};
    bannerTexture = nullptr;
    bannerSurface = nullptr;
    secondCounter = 0;
    alpha = 0;
}
// Fades in image
// Renderer, deltaTime
void IMG::drawFadeIn(SDL_Renderer *mRenderer, float deltaTime)
{
    secondCounter += deltaTime;
    if (secondCounter >= 0.025)
    {
        secondCounter = 0.0f;
        if (alpha < 255)
            alpha += 5;
    }
    SDL_DestroyTexture(bannerTexture);
    bannerSurface = IMG_Load(fileName);
    bannerTexture = SDL_CreateTextureFromSurface(mRenderer, bannerSurface);
    SDL_FreeSurface(bannerSurface);
    SDL_SetTextureAlphaMod(bannerTexture, alpha);
    SDL_RenderCopy(mRenderer, bannerTexture, NULL, &banner);
}

IMG::~IMG()
{
    SDL_DestroyTexture(bannerTexture);
}

void IMG::setAlpha(int x)
{
    alpha = x;
}