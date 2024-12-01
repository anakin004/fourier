#include "ini.hpp"
#include "Object.hpp"
#include "app.hpp"
#include <stdio.h>

SDL_Window *gWindow=nullptr;
SDL_Renderer *gRenderer=nullptr;
Object arrow;

const int WIDTH = 1200;
const int HEIGHT = 800;

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
        success = false;
        printf("Couldn't initialize video");
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("Fourier", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            success = false;
            printf("Could not create window %s", SDL_GetError());
        } else {
            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG  ) {
                success = false;
                printf(" wrong goofy ");
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else{
                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    if (!arrow.loadFromFile("arrow.png")) {
        printf("Failed to load player texture image!\n");
        success = false;
    }

    return success;

}
