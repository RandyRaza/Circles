#include "application.h"
#include "sdl2.h"
#include <stdio.h>

struct application *application_initialize() {
    struct application *application;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        return NULL;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!");
    }
    application = malloc(sizeof(struct application));
    application->window = SDL_CreateWindow("Maze",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (application->window == NULL) {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        return NULL;
    }
    application->renderer = SDL_CreateRenderer(application->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (application->renderer == NULL) {
        fprintf(stderr, "Renderer could not be created: %s\n", SDL_GetError());
        return NULL;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image failed to initialize: %s\n", IMG_GetError());
        return NULL;
    }
    application->menu = menu_initialize(application->renderer);
    if (application->menu == NULL) {
        fprintf(stderr, "Failed to initialize menu: %s\n", IMG_GetError());
        return NULL;
    }
    application->state = APPLICATION_STATE_MENU;
    return application;
}

void application_run(struct application *application) {
    while (application->state != APPLICATION_STATE_QUIT) {
        switch (application->state) {
            case APPLICATION_STATE_MENU:
                menu_run(application->menu);
                if (application->menu->state == MENU_QUIT) {
                    application->state = APPLICATION_STATE_QUIT;
                } else if (application->menu->state == MENU_PLAY) {
                    application->state = APPLICATION_STATE_QUIT;
                }
                break;
            case APPLICATION_STATE_PLAY:
                break;
            case APPLICATION_STATE_QUIT:
                break;
        }
    }
}

void application_shut_down(struct application *application) {
    SDL_DestroyRenderer(application->renderer);
    SDL_DestroyWindow(application->window);
    menu_delete(application->menu);
    free(application);
    application = NULL;
    IMG_Quit();
    SDL_Quit();
}
