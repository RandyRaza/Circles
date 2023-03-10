CC = gcc
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = -Wall -Wextra `sdl2-config --cflags`
LDFLAGS = -lSDL2_image `sdl2-config --libs`
EXEC = circles

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
