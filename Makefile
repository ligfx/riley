CC=clang
override CFLAGS := -fvisibility=hidden -fpic -ansi -Wall -Wno-comment -Werror -g $(CFLAGS)

all: libriley.so libriley-sdl.so libriley-cairo.so include c16topng

include: riley.h sdl.h cairo.h
	@mkdir -p include/riley
	@echo " CP $^ => include/riley/"
	@cp $^ include/riley

c16topng: c16topng.o libriley.so libriley-cairo.so
	@${CC} $^ `pkg-config cairo glib-2.0 --libs` -L. -lriley -lriley-cairo -o $@ -Wl,-rpath,. ${LDFLAGS}
	@echo " LD $^ => $@"

c16topng.o: include c16topng.c
	@${CC} -c -Iinclude c16topng.c ${CFLAGS} `pkg-config glib-2.0 --cflags`
	@echo " CC c16topng.c => $@"

libriley.so: riley.o
	@${CC} -shared $^ -o $@ ${LDFLAGS}
	@echo " LD $^ => $@"

libriley-sdl.so: sdl.o libriley.so
	@${CC} -shared $^ -o $@ -L. -lriley ${LDFLAGS}
	@echo " LD $^ => $@"

libriley-cairo.so: cairo.o libriley.so
	@${CC} -shared $^ -o $@ -L. -lriley ${LDFLAGS}
	@echo " LD $^ => $@"

%.o: %.c
	@${CC} -g -c $^ ${CFLAGS}
	@echo " CC $^ => $@"

clean:
	-rm c16topng libriley.so libriley-sdl.so libriley-cairo.so
	-rm -frv *.o
	-rm -frv include
	-rm *~
