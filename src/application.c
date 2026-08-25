#include "application.h"
#include "sdl2.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>

struct application *application_initialize() {
    struct application *application;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        return NULL;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!");
    }

    // Seeded once here rather than in play_initialize, since play_initialize
    // is called again every time a new game starts and reseeding too close
    // together would make wall placement less random.
    srand((unsigned int)time(NULL));

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize: %s\n", Mix_GetError());
        return NULL;
    }
    Mix_Music *music = Mix_LoadMUS("../assets/courtesy.mp3");
    if (music == NULL) {
        fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
        return NULL;
    }
    Mix_PlayMusic(music, -1);

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
    // Default difficulty until the player actually picks one in the menu.
    application->play = play_initialize(application->renderer, PLAY_MEDIUM);
    if (application->play == NULL) {
        fprintf(stderr, "Failed to initialize play: %s\n", IMG_GetError());
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
                    application->state = APPLICATION_STATE_PLAY;
                    // Replace the placeholder play session with a fresh one
                    // that actually uses the difficulty the player confirmed.
                    play_delete(application->play);
                    application->play = play_initialize(application->renderer,
                                                         application->menu->selected_difficulty);
                    if (application->play == NULL) {
                        fprintf(stderr, "Failed to initialize play!\n");
                        application->state = APPLICATION_STATE_QUIT;
                    }
                }
                break;
            case APPLICATION_STATE_PLAY:
                play_run(application->play);
                if (application->play->state == PLAY_QUIT) {
                    application->state = APPLICATION_STATE_QUIT;
                } else if (application->play->state == PLAY_LOST ||
                           application->play->state == PLAY_BACK_TO_MENU) {
                    // Reset the menu's focus so menu_run's own loop
                    // (while state != MENU_PLAY && state != MENU_QUIT)
                    // runs again instead of exiting immediately.
                    application->menu->state = MENU_PLAY_FOCUS;
                    application->state = APPLICATION_STATE_MENU;
                }
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
    play_delete(application->play);
    free(application);
    application = NULL;
    IMG_Quit();
    SDL_Quit();
}
