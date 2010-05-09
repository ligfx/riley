#ifndef RILEY_SDL_H
#define RILEY_SDL_H

#include "riley.h"
#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif


RILEY_API SDL_Surface* c16_get_sprite_sdl (C16*, uint16_t);

#ifdef __cplusplus
}
#endif

#endif // RILEY_SDL_H
