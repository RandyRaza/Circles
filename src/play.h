//
// Created by Randy Raza on 2023-03-31.
//

#ifndef PLAY_H
#define PLAY_H

#include "sdl2.h"
#include "constants.h"
#include <stdbool.h>


enum play_state {

    PLAY_QUIT_FOCUS,       // Quit choice has focus
    PLAY_QUIT,              // Play has been chosen

};

struct play {
    enum play_state state;
    struct spritesheet *background2; // The menu background
    struct spritesheet *quit;
    int triangle_x; // Coordonnée x du triangle
    int triangle_y; // Coordonnée y du triangle
    double triangle_angle; // L'angle du triangle en degrés
    SDL_Renderer *renderer;
};



struct play *play_initialize(SDL_Renderer *renderer);

void play_run(struct play *play);

void play_delete(struct play *play);

#endif //PLAY_H
