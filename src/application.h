#ifndef APPLICATION_H
#define APPLICATION_H

#include "sdl2.h"
#include "constants.h"
#include "menu.h"
#include "play.h"

// --------------- //
// Data structures //
// --------------- //

enum application_state {
    APPLICATION_STATE_MENU, // We show the menu
    APPLICATION_STATE_PLAY, // We are playing
    APPLICATION_STATE_QUIT  // We are quitting
};

struct application {
    enum application_state state; // The current state
    struct menu *menu;            // The home menu
    struct play *play;
    SDL_Window* window;           // The window
    SDL_Renderer* renderer;       // The renderer
};

// --------- //
// Functions //
// --------- //

/**
 * Creates a new application
 *
 * @return  A pointer to a new application, NULL if there was an error
 */
struct application *application_initialize();

/**
 * Start running the application
 *
 * @param application  The application to run
 */
void application_run(struct application *application);

/**
 * Closes the given application
 *
 * @param application  The application to be closed
 */
void application_shut_down(struct application *application);

#endif
