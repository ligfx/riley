#include <riley/riley.h>
#include <riley/riley-sdl.h>

#include <SDL/SDL.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp = fopen ("test.c16", "r");
  assert (fp);
  C16 *image = c16_new_from_file (fp);
  assert (image);
  
  assert (SDL_Init (SDL_INIT_VIDEO) == 0);
  SDL_Surface *screen = SDL_SetVideoMode (100, 100, 32, 0);
  assert (screen);
  
  SDL_Surface *sprite = c16_get_sprite_sdl (image, 2);
  assert (sprite);
  
  SDL_BlitSurface (sprite, NULL, screen, NULL);
  SDL_Flip (screen);
  
  SDL_Delay (2000);
  
  SDL_FreeSurface (sprite);
  SDL_Quit ();
  return 0;
}
