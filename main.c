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
/*
  In Ronnie, this might be:
  
  sdl: init
  sdl: vdeo 100 100 32 0
  sdl: blit c16: sdl_ c16: file "test.c16" 2
  sdl: flip
  wait 2000
  sdl: quit
*/

  FILE *fp;
  c16_t *c16;
  c16_t *s16;
  blk_t *blk;
  
  {
    fp = fopen ("test.c16", "r");
    assert (fp);
    c16 = c16_new_from_file (fp);
    assert (c16);
  }
  {
    fp = fopen ("test.s16", "r");
    assert (fp);
    s16 = s16_new_from_file (fp);
    assert (s16);
  }
  {
    fp = fopen ("test.blk", "r");
    assert (fp);
    blk = blk_new_from_file (fp);
    assert (blk);
  }
  
  assert (SDL_Init (SDL_INIT_VIDEO) == 0);
  SDL_Surface *screen = SDL_SetVideoMode (100, 100, 32, 0);
  assert (screen);
  
  SDL_Surface *sprite;
  
  /*for (int i = 0; i < 3; ++i) {
    switch (i) {
    case 0:
      sprite = c16_get_sprite_sdl (c16, 2); break;
    case 1:
      sprite = c16_get_sprite_sdl (s16, 0); break;
    case 2:
      sprite = blk_get_sdl (blk); break;
    }
    assert (sprite);
    
    SDL_BlitSurface (sprite, NULL, screen, NULL);
    SDL_Flip (screen);
    SDL_Delay (2000);
    SDL_FreeSurface (sprite);
  }*/
  
  for (int i = 0; i < c16_get_number_of_sprites (c16); ++i)
  {
    sprite = c16_get_sprite_sdl (c16, i);
    assert (sprite);
    
    SDL_BlitSurface (sprite, NULL, screen, NULL);
    SDL_Flip (screen);
    SDL_Delay (100);
    SDL_FreeSurface (sprite);
  }
  
  SDL_Quit ();
  return 0;
}
