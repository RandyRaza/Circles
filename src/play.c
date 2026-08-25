#include "play.h"
#include "constants.h"
#include "sdl2.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// NUM_SECTORS and SECTOR_ANGLE come from constants.h (6 and 60 respectively).
// The hub is divided into 6 wedges; every wall leaves exactly one wedge open.

static void play_apply_difficulty(struct play *play) {
    switch (play->difficulty) {
        case PLAY_EASY:
            play->wall_speed = 120.0;
            play->spawn_interval = 1.4;
            break;
        case PLAY_HARD:
            play->wall_speed = 260.0;
            play->spawn_interval = 0.7;
            break;
        case PLAY_MEDIUM:
        default:
            play->wall_speed = 180.0;
            play->spawn_interval = 1.0;
            break;
    }
}

struct play *play_initialize(SDL_Renderer *renderer, enum play_difficulty difficulty) {
    struct play *play = malloc(sizeof(struct play));
    if (play == NULL) {
        return NULL;
    }

    play->renderer = renderer;
    play->background2 = spritesheet_create(BACKGROUND2_FILENAME, 1, 1, 1, renderer);
    play->quit = spritesheet_create(QUIT_FILENAME, 1, 1, 1, renderer);

    play->state = PLAY_RUNNING;
    play->difficulty = difficulty;
    play->player_angle = 270.0; // start at the top of the circle

    play->wall_count = 0;
    play->survived_time = 0.0;
    play->last_tick = SDL_GetTicks();

    play_apply_difficulty(play);
    play->spawn_timer = 0.0; // spawn the very first wall right away

    return play;
}

// Draws the current frame: background, walls (farthest to nearest so nearer
// walls paint over farther ones, which is what creates the "ring" look),
// the central hub, and the player's triangle.
static void play_render(struct play *play) {
    int cx = SCREEN_WIDTH / 2;
    int cy = SCREEN_HEIGHT / 2;

    SDL_SetRenderDrawColor(play->renderer, 0x12, 0x12, 0x18, 0xFF);
    SDL_RenderClear(play->renderer);
    spritesheet_render(play->background2, BACKGROUND2_X, BACKGROUND2_Y, 255, 0);

    for (int i = play->wall_count - 1; i >= 0; i--) {
        struct wall *w = &play->walls[i];
        if (!w->active) {
            continue;
        }
        for (int s = 0; s < NUM_SECTORS; s++) {
            if (s == w->open_sector) {
                continue; // this is the gap the player must line up with
            }
            filledPieRGBA(play->renderer, cx, cy, (Sint16)w->radius,
                          s * SECTOR_ANGLE, (s + 1) * SECTOR_ANGLE,
                          210, 70, 70, 255);
        }
    }

    filledCircleRGBA(play->renderer, cx, cy, CENTER_RADIUS, 200, 200, 200, 255);

    // Player triangle: defined in local space (tip pointing along +x),
    // then rotated by the player's current angle and translated onto
    // the circle of radius PLAYER_RADIUS around the hub.
    double rad = play->player_angle * M_PI / 180.0;
    double px = cx + PLAYER_RADIUS * cos(rad);
    double py = cy + PLAYER_RADIUS * sin(rad);

    double local_x[3] = { PLAYER_SIZE, -PLAYER_SIZE * 0.6, -PLAYER_SIZE * 0.6 };
    double local_y[3] = { 0.0, PLAYER_SIZE * 0.7, -PLAYER_SIZE * 0.7 };
    Sint16 sx[3], sy[3];
    for (int k = 0; k < 3; k++) {
        double rx = local_x[k] * cos(rad) - local_y[k] * sin(rad);
        double ry = local_x[k] * sin(rad) + local_y[k] * cos(rad);
        sx[k] = (Sint16)(px + rx);
        sy[k] = (Sint16)(py + ry);
    }
    filledTrigonRGBA(play->renderer, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2],
                      250, 220, 80, 255);

    SDL_RenderPresent(play->renderer);
}

void play_run(struct play *play) {
    SDL_Event e;
    play->last_tick = SDL_GetTicks();

    while (play->state == PLAY_RUNNING) {
        Uint32 now = SDL_GetTicks();
        double dt = (now - play->last_tick) / 1000.0;
        play->last_tick = now;
        // Guard against huge dt spikes (e.g. the window was dragged/paused)
        if (dt > 0.1) {
            dt = 0.1;
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                play->state = PLAY_QUIT;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                play->state = PLAY_BACK_TO_MENU;
            }
        }
        if (play->state != PLAY_RUNNING) {
            break;
        }

        // --- input: move the player around the hub ---
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) {
            play->player_angle -= PLAYER_ANGULAR_SPEED * dt;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            play->player_angle += PLAYER_ANGULAR_SPEED * dt;
        }
        if (play->player_angle < 0) {
            play->player_angle += 360.0;
        }
        if (play->player_angle >= 360.0) {
            play->player_angle -= 360.0;
        }
        int player_sector = ((int)play->player_angle / SECTOR_ANGLE) % NUM_SECTORS;

        // --- spawn new walls ---
        play->spawn_timer -= dt;
        if (play->spawn_timer <= 0.0 && play->wall_count < MAX_WALLS) {
            struct wall *w = &play->walls[play->wall_count++];
            w->radius = WALL_SPAWN_RADIUS;
            w->open_sector = rand() % NUM_SECTORS;
            w->active = true;
            play->spawn_timer += play->spawn_interval;
        }

        // --- move walls inward, check collision, deactivate passed walls ---
        for (int i = 0; i < play->wall_count; i++) {
            struct wall *w = &play->walls[i];
            if (!w->active) {
                continue;
            }
            w->radius -= play->wall_speed * dt;

            bool touching_player = w->radius <= PLAYER_RADIUS + WALL_THICKNESS &&
                                    w->radius + WALL_THICKNESS >= PLAYER_RADIUS;
            if (touching_player && w->open_sector != player_sector) {
                play->state = PLAY_LOST;
            }
            if (w->radius < CENTER_RADIUS) {
                w->active = false;
            }
        }

        // compact the array so inactive walls don't pile up forever
        int write = 0;
        for (int i = 0; i < play->wall_count; i++) {
            if (play->walls[i].active) {
                play->walls[write++] = play->walls[i];
            }
        }
        play->wall_count = write;

        play->survived_time += dt;

        if (play->state != PLAY_RUNNING) {
            break; // the player just lost this frame, skip the render below
        }

        play_render(play);
    }

    if (play->state == PLAY_LOST) {
        fprintf(stderr, "Game over — survived %.1f seconds\n", play->survived_time);
    }
}

void play_delete(struct play *play) {
    if (play != NULL) {
        spritesheet_delete(play->background2);
        spritesheet_delete(play->quit);
        free(play);
    }
}
