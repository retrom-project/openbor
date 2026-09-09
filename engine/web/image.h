#ifndef RETROM_WEB_IMAGE_H
#define RETROM_WEB_IMAGE_H
#include <SDL.h>
#include <stddef.h>
SDL_Surface *retrom_gif_load(const unsigned char *bytes, size_t size);
#endif
