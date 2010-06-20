#include "riley.h"
#include "sdl.h"

SDL_Surface*
make_surface (uint16_t *data, int width, int height, c16_format_t format)
{
	unsigned int rmask, gmask, bmask;
	if (format == C16_565)
		rmask = 0xf800, gmask = 0x07e0, bmask = 0x001f;
	else
		rmask = 0x7c00, gmask = 0x03e0, bmask = 0x001f;
  
	return SDL_CreateRGBSurfaceFrom(
	  data, width, height,
		16, // depth
		width * 2, // pitch
		rmask, gmask, bmask, 0);
}

SDL_Surface*
c16_get_sprite_sdl (c16_t *image, uint16_t index)
{
  c16_sprite_t sprite = c16_get_sprite (image, index);
  
  return make_surface (
    c16_sprite_get_data (sprite),
    c16_sprite_get_width (sprite),
    c16_sprite_get_height (sprite),
    c16_get_format (image));
}

SDL_Surface*
blk_get_sdl (blk_t *image)
{
  return make_surface (
    blk_get_data (image),
    blk_get_width (image),
    blk_get_height (image),
    blk_get_format (image));
}
