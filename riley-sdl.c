#include "riley.h"
#include "riley-sdl.h"

SDL_Surface*
c16_get_sprite_sdl (C16 *image, uint16_t index)
{
  C16Sprite sprite = c16_get_sprite (image, index);
  C16Format format = c16_get_format (image);
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
