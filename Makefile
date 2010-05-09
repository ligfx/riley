CC=clang
CFLAGS +=-fvisibility=hidden -fpic -ansi -Wall -Wno-comment -Werror

all: libriley.so libriley-sdl.so include main

include: riley.h riley-sdl.h
	@mkdir -p include/riley
	@echo " CP $^ => include/riley/"
	@cp $^ include/riley

main: main.o libriley.so libriley-sdl.so
	@${CC} -g $^ -lSDL -L. -lriley -lriley-sdl -o $@ -Wl,-rpath,.
	@echo " LD $^ => $@"

main.o: include main.c
	@${CC} -g -c -Iinclude main.c ${CFLAGS}
	@echo " CC main.c => $@"

libriley.so: riley.o
	@${CC} -shared $^ -o $@
	@echo " LD $^ => $@"

libriley-sdl.so: riley-sdl.o libriley.so
	@${CC} -shared $^ -o $@ -L. -lriley
	@echo " LD $^ => $@"

%.o: %.c
	@${CC} -g -c $^ ${CFLAGS}
	@echo " CC $^ => $@"

clean:
	-rm main libriley.so libriley-sdl.so
	-rm -frv *.o
	-rm -frv include
