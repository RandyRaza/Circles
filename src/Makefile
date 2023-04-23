CC = gcc
CFLAGS = -Wall -Wextra -D_THREAD_SAFE `sdl2-config --cflags` `pkg-config --cflags SDL2_gfx`
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx `sdl2-config --libs` `pkg-config --libs SDL2_gfx`

SDL_DIR = /usr/local/SDL2
SDL_GFX_DIR = /usr/local/Cellar/sdl2_gfx/1.0.4

EXEC = circles
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
