#ifndef RILEY_CAIRO_H
#define RILEY_CAIRO_H

#include "riley.h"
#include <cairo/cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

RILEY_API cairo_surface_t* c16_get_sprite_cairo (c16_t*, uint16_t);
RILEY_API cairo_surface_t* blk_get_cairo (blk_t*);

#ifdef __cplusplus
}
#endif

#endif // RILEY_SDL_H
