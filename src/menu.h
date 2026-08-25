#ifndef MENU_H
#define MENU_H

#include "sdl2.h"
#include "spritesheet.h"
#include "play.h"

// NOTE: reconstruit à partir de l'usage réel dans menu.c (tu ne m'as pas
// encore montré ton menu.h d'origine). Si ton fichier a d'autres champs
// ou fonctions que je n'ai pas vus, fusionne-les avec cette version plutôt
// que d'écraser — le seul ajout volontaire ici est `selected_difficulty`.

enum menu_state {
    MENU_PLAY_FOCUS,
    MENU_QUIT_FOCUS,
    MENU_EASY_FOCUS,
    MENU_MEDIUM_FOCUS,
    MENU_HARD_FOCUS,
    MENU_PLAY,
    MENU_QUIT
};

struct menu {
    SDL_Renderer *renderer;
    enum menu_state state;

    struct spritesheet *background;
    struct spritesheet *title;
    struct spritesheet *play;
    struct spritesheet *quit;
    struct spritesheet *easy;
    struct spritesheet *medium;
    struct spritesheet *hard;
    struct spritesheet *difficulty;

    // NEW: which difficulty is actually confirmed (as opposed to merely
    // highlighted by the arrow keys). Defaults to PLAY_MEDIUM.
    enum play_difficulty selected_difficulty;
};

struct menu *menu_initialize(SDL_Renderer *renderer);
void menu_run(struct menu *menu);
void menu_delete(struct menu *menu);

#endif
