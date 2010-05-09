#ifndef RILEY_H
#define RILEY_H

#include <stdint.h>
#include <stdio.h>

#include <SDL/SDL.h>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #define RILEY_API __declspec(dllexport)
  #else
    #define RILEY_API __declspec(dllimport)
  #endif
#else
  #if __GNUC__ >= 4
    #define RILEY_API __attribute__ ((visibility("default")))
  #else
    #define RILEY_API
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum C16Format {
	C16_555,
	C16_565,
} C16Format;

typedef struct C16Sprite {
  uint16_t width, height;
  uint16_t* data;
} C16Sprite;

typedef struct C16 {
  C16Format format;
  uint16_t count;
  C16Sprite *sprites;
} C16;

RILEY_API C16* c16_new(C16Format);

RILEY_API C16Format c16_get_format (C16*);

RILEY_API uint16_t c16_get_number_of_sprites (C16*);
RILEY_API void c16_set_number_of_sprites (C16*, uint16_t);

RILEY_API C16Sprite c16_get_sprite (C16*, uint16_t);
RILEY_API void c16_set_sprite (C16*, uint16_t, C16Sprite);

RILEY_API C16Sprite c16_sprite_make (uint16_t width, uint16_t height, uint16_t* data);
RILEY_API uint16_t c16_sprite_get_width (C16Sprite);
RILEY_API uint16_t c16_sprite_get_height (C16Sprite);

RILEY_API uint16_t* c16_sprite_get_data (C16Sprite);

// ~ Helpers ~

RILEY_API C16* c16_new_from_file (FILE*);

#ifdef __cplusplus
}
#endif

#endif // RILEY_H
