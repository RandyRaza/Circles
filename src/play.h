#ifndef PLAY_H
#define PLAY_H

#include "sdl2.h"
#include "spritesheet.h"
#include <stdbool.h>

// --------- //
// Constants //
// --------- //

#define MAX_WALLS 64

#define PLAYER_RADIUS   110  // distance from the center where the triangle sits
#define PLAYER_SIZE     14   // size of the player's triangle
#define CENTER_RADIUS   40   // radius of the central hub
#define WALL_THICKNESS  18   // thickness (in px) of a wall band
#define WALL_SPAWN_RADIUS 620 // distance from the center where new walls appear
#define PLAYER_ANGULAR_SPEED 220.0 // degrees / second

// --------------- //
// Data structures //
// --------------- //

enum play_difficulty {
    PLAY_EASY,
    PLAY_MEDIUM,
    PLAY_HARD
};

enum play_state {
    PLAY_RUNNING,       // the game is being played
    PLAY_LOST,          // the player hit a wall
    PLAY_BACK_TO_MENU,  // the player pressed ESC
    PLAY_QUIT           // the player closed the window
};

// A single ring of walls, closing in towards the center.
// One sector (out of NUM_SECTORS) is left open as the gap the player
// must be aligned with when the wall reaches PLAYER_RADIUS.
struct wall {
    double radius;
    int open_sector;
    bool active;
};

struct play {
    SDL_Renderer *renderer;
    struct spritesheet *background2;
    struct spritesheet *quit;

    enum play_state state;
    enum play_difficulty difficulty;

    double player_angle; // 0-360 degrees, position of the triangle around the hub

    struct wall walls[MAX_WALLS];
    int wall_count;

    double wall_speed;     // px / second, how fast walls move inward (set from difficulty)
    double spawn_timer;    // seconds left until the next wall spawns
    double spawn_interval; // seconds between spawns (set from difficulty)

    double survived_time; // seconds survived so far, used as the score
    Uint32 last_tick;     // SDL_GetTicks() at the last processed frame
};

// --------- //
// Functions //
// --------- //

/**
 * Creates a new play session at the given difficulty.
 *
 * @param renderer    The renderer to draw with
 * @param difficulty  The chosen difficulty, controls wall speed and spawn rate
 * @return  A pointer to a new play session, NULL if there was an error
 */
struct play *play_initialize(SDL_Renderer *renderer, enum play_difficulty difficulty);

/**
 * Runs the play session until the player loses, quits to the menu,
 * or closes the window. This function contains its own game loop,
 * the same way menu_run contains its own event loop.
 *
 * @param play  The play session to run
 */
void play_run(struct play *play);

/**
 * Frees the given play session.
 *
 * @param play  The play session to be freed
 */
void play_delete(struct play *play);

#endif
