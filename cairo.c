#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "riley.h"
#include "cairo.h"

void
riley_copy_to_argb32 (
  uint16_t *sprite_data,
  uint16_t width,
  uint16_t height,
  c16_format_t format,
  uint32_t *data,
  int stride)
{
	for (int i = 0; i < height; ++i) {
	  unsigned char *ptr = (unsigned char*) (data + i * stride / 4);
	  for (int j = 0; j < width; ++j) {
	    uint16_t pixel = *(sprite_data++);
      // TODO: This is dependent on endianness
      if (format == C16_565) {
        *(ptr++) = (pixel & 0x001f) << 3; // Blue
        *(ptr++) = (pixel & 0x07e0) >> 3; // Green
        *(ptr++) = (pixel & 0xf800) >> 8; // Red
      } else {
        *(ptr++) = (pixel & 0x001f) << 3; // Blue
        *(ptr++) = (pixel & 0x03e0) >> 2; // Green
        *(ptr++) = (pixel & 0x7c00) >> 7; // Red
      }
      *(ptr++) = 255; // opaque
	  }
	}
}

cairo_surface_t*
c16_get_sprite_cairo (c16_t *c16, uint16_t index)
{
  c16_sprite_t sprite = c16_get_sprite (c16, 0);

	int height = c16_sprite_get_height (sprite);
	int width = c16_sprite_get_width (sprite);
	
	cairo_surface_t *surface = cairo_image_surface_create
	  (CAIRO_FORMAT_ARGB32, width, height);
	
	riley_copy_to_argb32 (
	  c16_sprite_get_data (sprite),
	  width, height,
	  c16_get_format (c16),
	  (uint32_t*) cairo_image_surface_get_data (surface),
	  cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, width));
	
	return surface;
}

cairo_surface_t*
blk_get_cairo (blk_t *blk)
{
	int height = blk_get_height (blk);
	int width = blk_get_width (blk);
	
	cairo_surface_t *surface = cairo_image_surface_create
	  (CAIRO_FORMAT_ARGB32, width, height);
	
	riley_copy_to_argb32 (
	  blk_get_data (blk),
	  width, height,
	  blk_get_format (blk),
	  (uint32_t*) cairo_image_surface_get_data (surface),
	  cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, width));
	
	return surface;
}

int main () {
  blk_t *blk = NULL;
  
  {
    FILE *fp = fopen ("test.blk", "r");
    assert (fp);
    blk = blk_new_from_file (fp);
    assert (blk);
  }
	
	cairo_surface_write_to_png (blk_get_cairo (blk), "test.png");
}
