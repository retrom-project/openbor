#include "image.h"
#include <assert.h>
#include <string.h>
int main(void) {
    /* Project-owned 2x1 indexed image: red, black. */
    unsigned char gif[] = {'G','I','F','8','9','a',2,0,1,0,128,0,0,0,0,0,255,0,0,
        44,0,0,0,0,2,0,1,0,0,2,2,12,10,0,59};
    SDL_Surface *image = retrom_gif_load(gif, sizeof(gif));
    assert(image && image->w == 2 && image->h == 1);
    assert(((unsigned char *)image->pixels)[0] == 1 && ((unsigned char *)image->pixels)[1] == 0);
    assert(image->format->palette->colors[1].r == 255);
    SDL_FreeSurface(image);
    gif[6] = 3; gif[20] = 1;
    image = retrom_gif_load(gif, sizeof(gif));
    assert(image && image->w == 3 && ((unsigned char *)image->pixels)[0] == 0 && ((unsigned char *)image->pixels)[1] == 1);
    SDL_FreeSurface(image);
    gif[6] = 2; gif[20] = 0;
    assert(!retrom_gif_load(gif, 25));
    gif[6] = 255; gif[7] = 255;
    assert(!retrom_gif_load(gif, sizeof(gif)));
    gif[6] = 2; gif[7] = 0; gif[30] = 200;
    assert(!retrom_gif_load(gif, sizeof(gif)));
    return 0;
}
