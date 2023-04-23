#include "spritesheet.h"
#include "sdl2.h"
#include <stdbool.h>
#include "play.h"
#include "constants.h"
#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>

struct play *play_initialize(SDL_Renderer *renderer) {
    struct play *play;
    play = malloc(sizeof(struct play));
    play->renderer = renderer;
    play->background2 = spritesheet_create(BACKGROUND2_FILENAME, 1, 1, 1, renderer);
    play->quit = spritesheet_create(QUIT_FILENAME, 1, 1, 1, renderer);
    play->triangle_x = SCREEN_WIDTH/2;
    play->triangle_y = SCREEN_HEIGHT/2 - 100;
    play->triangle_angle = 0;

    SDL_RenderPresent(play->renderer);
    return play;
}

int play_alpha(bool chosen) {
    return chosen ? 255 : 64;
}

void play_run(struct play *play) {
    SDL_SetRenderDrawColor(play->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(play->renderer);
    filledCircleRGBA(play->renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 40, 128, 128, 128, 255);

    SDL_RenderPresent(play->renderer);
}

void play_delete(struct play *play) {
    if (play != NULL) {
        spritesheet_delete(play->background2);
        spritesheet_delete(play->quit);
        free(play);
    }
}
