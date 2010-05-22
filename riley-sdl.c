#include "riley.h"
#include "riley-sdl.h"

SDL_Surface*
c16_get_sprite_sdl (c16_t *image, uint16_t index)
{
  c16_sprite_t sprite = c16_get_sprite (image, index);
  c16_format_t format = c16_get_format (image);
  uint16_t width, height, *data;
  width = c16_sprite_get_width (sprite);
  height = c16_sprite_get_height (sprite);
  data = c16_sprite_get_data (sprite);
  
	unsigned int rmask, gmask, bmask;
	if (format == C16_565)
		rmask = 0xF800, gmask = 0x07E0, bmask = 0x001F;
	else
		rmask = 0x7C00, gmask = 0x03E0, bmask = 0x001F;
  
	return SDL_CreateRGBSurfaceFrom(
	  data, width, height,
		16, // depth
		width * 2, // pitch
		rmask, gmask, bmask, 0); // RGBA mask
}

SDL_Surface*
blk_get_sdl (blk_t *image)
{
  c16_format_t format = image->format;
  uint16_t width, height, *data;
  width = image->width;
  height = image->height;
  data = image->data;
  
	unsigned int rmask, gmask, bmask;
	if (format == C16_565)
		rmask = 0xF800, gmask = 0x07E0, bmask = 0x001F;
	else
		rmask = 0x7C00, gmask = 0x03E0, bmask = 0x001F;
  
	return SDL_CreateRGBSurfaceFrom(
	  data, width, height,
		16, // depth
		width * 2, // pitch
		rmask, gmask, bmask, 0); // RGBA mask
}
