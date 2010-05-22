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

typedef enum c16_format_t {
	C16_555,
	C16_565,
} c16_format_t;

typedef struct c16_sprite_t {
  uint16_t width, height;
  uint16_t* data;
} c16_sprite_t;

typedef struct c16_t {
  c16_format_t format;
  uint16_t count;
  c16_sprite_t *sprites;
} c16_t;

RILEY_API c16_t* c16_new (c16_format_t);

RILEY_API c16_format_t c16_get_format (c16_t*);

RILEY_API uint16_t c16_get_number_of_sprites (c16_t*);
RILEY_API void c16_set_number_of_sprites (c16_t*, uint16_t);

RILEY_API c16_sprite_t c16_get_sprite (c16_t*, uint16_t);
RILEY_API void c16_set_sprite (c16_t*, uint16_t, c16_sprite_t);

RILEY_API c16_sprite_t c16_sprite_make (uint16_t width, uint16_t height, uint16_t* data);
RILEY_API uint16_t c16_sprite_get_width (c16_sprite_t);
RILEY_API uint16_t c16_sprite_get_height (c16_sprite_t);

RILEY_API uint16_t* c16_sprite_get_data (c16_sprite_t);

typedef struct blk_t {
  c16_format_t format;
  uint16_t width, height;
  uint16_t* data;
} blk_t;

RILEY_API blk_t* blk_new(c16_format_t, uint16_t width, uint16_t height, uint16_t *data);
RILEY_API c16_format_t blk_get_format (blk_t*);
RILEY_API uint16_t blk_get_width (blk_t*);
RILEY_API uint16_t blk_get_height (blk_t*);
RILEY_API uint16_t* blk_get_data (blk_t*);

// ~ Helpers ~

RILEY_API c16_t* c16_new_from_file (FILE*);
//RILEY_API void c16_write_to_file (C16*, FILE*);
RILEY_API c16_t* s16_new_from_file (FILE*);
//RILEY_API void s16_write_to_file (C16*, FILE*);
RILEY_API blk_t* blk_new_from_file (FILE*);
//RILEY_API void blk_write_to_file (BLK*, FILE*);

#ifdef __cplusplus
}
#endif

#endif // RILEY_H
