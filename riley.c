#include "riley.h"

#include <assert.h>
#include <stdbool.h>

#define ENSURE(condition) if (!(condition)) return NULL

c16_t*
c16_new (c16_format_t format)
{
  c16_t *image = malloc (sizeof (c16_t)); {
    image->format = format;
    image->count = 0;
    image->sprites = NULL;
  }
  return image;
}

void
c16_destroy (c16_t *c16)
{
  for (int i = 0; i < c16->count; ++i)
    free (c16->sprites[i].data);
  free (c16);
}

c16_format_t
c16_get_format (c16_t* image)
{
  return image->format;
}

c16_sprite_t
c16_sprite_make (uint16_t width, uint16_t height, uint16_t* data)
{
  c16_sprite_t sprite = { width, height, data };
  return sprite;
}

uint16_t
c16_get_number_of_sprites (c16_t *image)
{
  return image->count;
}

void
c16_set_number_of_sprites (c16_t *image, uint16_t count)
{
  image->count = count;
  image->sprites = realloc (image->sprites, count * sizeof (c16_sprite_t));
}

c16_sprite_t
c16_get_sprite (c16_t *image, uint16_t index)
{
  assert (index < image->count);
  return image->sprites[index];
}

void
c16_set_sprite (c16_t *image, uint16_t index, c16_sprite_t sprite)
{
  assert (index < image->count);
  image->sprites[index] = sprite;
}

uint16_t*
c16_sprite_get_data (c16_sprite_t sprite)
{
  return sprite.data;
}

uint16_t
c16_sprite_get_width (c16_sprite_t sprite)
{
  return sprite.width;
}

uint16_t
c16_sprite_get_height (c16_sprite_t sprite)
{
  return sprite.height;
}

blk_t*
blk_new (c16_format_t format, uint16_t width, uint16_t height, uint16_t *data)
{
  blk_t *blk = malloc (sizeof (blk_t)); {
    blk->format = format;
    blk->width = width;
    blk->height = height;
    blk->data = data;
  }
  return blk;
}

void
blk_destroy (blk_t *blk)
{
  free (blk->data);
  free (blk);
}

c16_format_t
blk_get_format (blk_t *blk)
{
  return blk->format;
}

uint16_t
blk_get_height (blk_t *blk)
{
  return blk->height;
}

uint16_t
blk_get_width (blk_t *blk)
{
  return blk->width;
}

uint16_t*
blk_get_data (blk_t *blk)
{
  return blk->data;
}

size_t
nice_read (FILE *fp, size_t n, void *buf)
{
  return fread (buf, 1, n, fp);
}

void
nice_seek (FILE *fp, size_t mark)
{
  fseek (fp, mark, SEEK_SET);
}

static struct riley_stream file_iface = {
  (riley_read_t) nice_read,
  (riley_seek_t) nice_seek,
  (riley_tell_t) ftell
};

c16_t*
c16_new_from_file (FILE *fp)
{
  return c16_new_from_stream (fp, file_iface);
}

c16_t*
s16_new_from_file (FILE *fp)
{
  return s16_new_from_stream (fp, file_iface);
}

blk_t*
blk_new_from_file (FILE *fp)
{
  return blk_new_from_stream (fp, file_iface); 
}

struct data_map {
  void *data;
  void *ptr;
};

size_t
data_map_read (struct data_map *map, size_t n, void *buf)
{
  memcpy (buf, map->ptr, n);
  map->ptr = ((char*)map->ptr) + n;
  return n;
}

void
data_map_seek (struct data_map *map, size_t mark)
{
  map->ptr = ((char*)map->data) + mark;
}

size_t
data_map_tell (struct data_map *map)
{
  return ((char*)map->ptr) - ((char*)map->data);
}

static struct riley_stream data_iface = {
  (riley_read_t) data_map_read,
  (riley_seek_t) data_map_seek,
  (riley_tell_t) data_map_tell
};

c16_t*
c16_new_from_data (void *data)
{
  struct data_map map = { data, data };
  return c16_new_from_stream (&map, data_iface);
}

c16_t*
s16_new_from_data (void *data)
{
  struct data_map map = { data, data };
  return s16_new_from_stream (&map, data_iface);
}

blk_t*
blk_new_from_data (void *data)
{
  struct data_map map = { data, data };
  return blk_new_from_stream (&map, data_iface);
}

#define READ(count, ptr) iface.read(stream, count, ptr)
#define SEEK(mark) iface.seek(stream, mark)
#define TELL(mark) iface.tell(stream)

c16_t*
c16_new_from_stream (void *stream, struct riley_stream iface)
{
  c16_t* image;

  uint32_t header;
  bool is_565;
  uint16_t count;
  
  ENSURE(READ(4, &header) == 4);
  
  is_565 = header & 0x1;
  ENSURE(header & 0x2); // Is a C16 File
  
  image = c16_new (is_565? C16_565 : C16_555);
  
  ENSURE (READ(2, &count) == 2);
  c16_set_number_of_sprites (image, count);
  
  for (int i = 0; i < count; ++i) {
    c16_sprite_t sprite;
    uint32_t first_line;
    uint16_t width, height;
    char *data = NULL;
    uint16_t *data_pos = NULL;
    int j;
    
    ENSURE (READ(4, &first_line) == 4);
    ENSURE (READ(2, &width) == 2);
    ENSURE (READ(2, &height) == 2);
    
    // TODO: Possible memory leak if a later ensure fails
    data = malloc (width * height * 2);
    data_pos = (uint16_t*)data;
    ENSURE (data);
    
    uint32_t offsets[height];
    offsets[0] = first_line;
    for (j = 1; j < height; ++j)
      ENSURE (READ(4, &offsets[j]) == 4);
    
    long mark = TELL();
    
    for (j = 0; j < height; ++j) {
      SEEK (offsets[j]);
      while (true) {
        uint16_t tag;
        ENSURE (READ (2, &tag) == 2);
        if (tag == 0) break;
        bool color = tag & 0x0001;
        uint16_t length = (tag & 0xFFFE) >> 1;
        if (color) {
          ENSURE (READ (length * 2, data_pos) == length * 2);
        } else {
          memset (data_pos, 0, length * 2);
        }
        data_pos += length;
      }
    }
    
    sprite = c16_sprite_make (width, height, (uint16_t*)data);
    c16_set_sprite (image, i, sprite);
    
    SEEK (mark);
  }
  
  return image;
}

c16_t*
s16_new_from_stream (void *stream, struct riley_stream iface)
{
  c16_t* image;

  uint32_t header;
  bool is_565;
  uint16_t count;
  
  ENSURE (READ (4, &header) == 4);
  
  is_565 = header & 0x1;
  ENSURE (header ^ 0x2); // Is an S16 File; not C16
  
  image = c16_new (is_565? C16_565 : C16_555);
  
  ENSURE (READ (2, &count) == 2);
  c16_set_number_of_sprites (image, count);
  
  for (int i = 0; i < count; ++i) {
    c16_sprite_t sprite;
    uint32_t first_line;
    uint16_t width, height;
    char *data = NULL;
    
    ENSURE (READ (4, &first_line) == 4);
    ENSURE (READ (2, &width) == 2);
    ENSURE (READ (2, &height) == 2);
    
    data = malloc (width * height * 2);
    ENSURE (data);
    
    long mark = TELL();
    SEEK (first_line);
    ENSURE (READ (width * height * 2, data) == width * height * 2);
    SEEK (mark);
    
    sprite = c16_sprite_make (width, height, (uint16_t*)data);
    c16_set_sprite (image, i, sprite);
  }
  
  return image;
}

blk_t*
blk_new_from_stream (void *stream, struct riley_stream iface)
{
  blk_t *blk = NULL;
  
  uint32_t header;
  bool is_565;
  uint16_t bgwidth, bgheight, count;
  char* data;
  int i;
  
  ENSURE (READ (4, &header) == 4);
  
  is_565 = header & 0x1;
  ENSURE (header ^ 0x2); // Is an S16 File; not C16
  
  ENSURE (READ (2, &bgwidth) == 2);
  ENSURE (READ (2, &bgheight) == 2);
  ENSURE (READ (2, &count) == 2);
  
  uint32_t offsets[count];
  for (i = 0; i < count; ++i) {
    uint32_t off;
    ENSURE (READ (4, &off) == 4);
    offsets[i] = off + 4; // Because of the extra width and height in header
    
    uint16_t width, height;
    ENSURE (READ (2, &width) == 2);
    ENSURE (READ (2, &height) == 2);
    ENSURE (width == 128);
    ENSURE (height == 128);
  }
  
  data = malloc (sizeof (uint16_t) * bgwidth * 128 * bgheight * 128);
  ENSURE (data);
  /*
    So sprites are arranged top-to-bottom, left-to-right. That is, sprite 1 is
    at (0,0) and sprite 2 is at (0, 1). Complicated? Yes
  */
  for (i = 0; i < count; ++i) { // For each sprite
    long mark = TELL (fp);
    SEEK (offsets[i]);
    
    for (int j = 0; j < 128; ++j) // For each pixel-row
      ENSURE (READ (256, data + j * bgwidth * 256 + (i % bgheight) * 128 * bgwidth * 256 + (i / bgheight) * 256) == 256);
    
    SEEK (mark);
  }
  
  blk = blk_new (is_565?C16_565:C16_555, bgwidth*128, bgheight*128, (uint16_t*)data);
  return blk;
}
