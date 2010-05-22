#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "riley.h"
#include "riley-cairo.h"

cairo_surface_t*
c16_get_sprite_cairo (C16 *c16, uint16_t index)
{
  C16Sprite sprite = c16_get_sprite (c16, 0);
  C16Format format = c16_get_format (c16);

	int height = c16_sprite_get_height (sprite);
	int width = c16_sprite_get_width (sprite);
	
	cairo_surface_t *surface = cairo_image_surface_create
	  (CAIRO_FORMAT_ARGB32, width, height);
	uint32_t* data = (uint32_t*) cairo_image_surface_get_data (surface);
	int stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, width);
	
	uint16_t *sprite_data = c16_sprite_get_data (sprite);
	for (int i = 0; i < height; ++i) {
	  for (int j = 0; j < width; ++j) {
	    uint16_t pixel = sprite_data[i*width + j];
      // TODO: This is dependent on endianness
      unsigned char *ptr = (unsigned char*) &data[i * stride / 4 + j];
      if (format == C16_565) {
        *(ptr++) = (pixel & 0x001f) << 3; // Blue
        *(ptr++) = (pixel & 0x07e0) >> 3; // Green
        *(ptr++) = (pixel & 0xf800) >> 8; // Red
      } else {
        *(ptr++) = (pixel & 0x001f) << 3; // Blue
        *(ptr++) = (pixel & 0x03e0) >> 2; // Green
        *(ptr++) = (pixel & 0x7c00) >> 7; // Red
      }
      *ptr = 255; // opaque
	  }
	}
	
	return surface;
}

void foo (uint16_t pixel, uint32_t *ptr) {
  // TODO: This is dependent on endianness
//  if (format == C16_565) {

  
/*    *(ptr++) = (pixel & 0x001f) << 3; // Blue
    *(ptr++) = (pixel & 0x07e0) >> 3; // Green
    *(ptr++) = (pixel & 0xf800) >> 8; // Red*/
/*  } else {
    *(ptr++) = (pixel & 0x001f) << 3; // Blue
    *(ptr++) = (pixel & 0x03e0) >> 2; // Green
    *(ptr++) = (pixel & 0x7c00) >> 7; // Red
  }*/
//  *(ptr++) = 255; // opaque
}

cairo_surface_t*
blk_get_cairo (BLK *blk)
{
  C16Format format = blk_get_format (blk);

	int height = blk_get_height (blk);
	int width = blk_get_width (blk);
	
	cairo_surface_t *surface = cairo_image_surface_create
	  (CAIRO_FORMAT_ARGB32, width, height);
	uint32_t* data = (uint32_t*) cairo_image_surface_get_data (surface);
	int stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, width);
	
	uint16_t *blk_data = blk_get_data (blk);
	for (int i = 0; i < height; ++i) {
	  uint32_t *ptr = data + (i * stride / 4);
	  for (int j = 0; j < width; ++j) {
//	    printf (".");
      uint16_t pixel = blk_data[i*width + j];
      *(ptr++) =  ((pixel & 0x001f) << 3) | // blue
                 (((pixel & 0x07e0) >> 3) << 8) | // green
                 (((pixel & 0xf800) >> 8) << 16) | // red
                                     (255 << 24); // alpha
	  }
	}
	
	return surface;
}

int main () {
  BLK *blk = NULL;
  
  {
    FILE *fp = fopen ("test.blk", "r");
    assert (fp);
    blk = blk_new_from_file (fp);
    assert (blk);
  }
	
	cairo_surface_write_to_png (blk_get_cairo (blk), "test.png");
}
