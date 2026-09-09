/* Retrom indexed GIF compatibility, using the pinned SDL_image decoder. */
#include "image.h"
#include <SDL_image.h>
#include <string.h>
#define GIF_MAX_BYTES (64U * 1024U * 1024U)
#define GIF_MAX_PIXELS (16U * 1024U * 1024U)
static unsigned word(const unsigned char *p) { return p[0] | ((unsigned)p[1] << 8); }
static int dimensions(unsigned w, unsigned h) {
    return w && h && w <= 8192 && h <= 8192 && w * h <= GIF_MAX_PIXELS;
}
static int blocks(const unsigned char *bytes, size_t size, size_t *position) {
    while (*position < size) {
        unsigned length = bytes[(*position)++];
        if (!length) return 1;
        if (length > size - *position) return 0;
        *position += length;
    }
    return 0;
}
typedef struct {unsigned w, h, left, top, fw, fh;} GifBounds;
static int valid_gif(const unsigned char *bytes, size_t size, GifBounds *bounds) {
    if (!bytes || size < 13 || size > GIF_MAX_BYTES ||
        (memcmp(bytes, "GIF87a", 6) && memcmp(bytes, "GIF89a", 6)) ||
        !dimensions(word(bytes + 6), word(bytes + 8))) return 0;
    bounds->w = word(bytes + 6); bounds->h = word(bytes + 8);
    size_t pos = 13;
    if (bytes[10] & 128) pos += 3U << ((bytes[10] & 7) + 1);
    while (pos < size) {
        unsigned marker = bytes[pos++];
        if (marker == 0x21) {
            if (pos >= size) return 0;
            pos++;
            if (!blocks(bytes, size, &pos)) return 0;
        } else if (marker == 0x2c) {
            if (size - pos < 9 || !dimensions(word(bytes + pos + 4), word(bytes + pos + 6))) return 0;
            bounds->left = word(bytes + pos); bounds->top = word(bytes + pos + 2);
            bounds->fw = word(bytes + pos + 4); bounds->fh = word(bytes + pos + 6);
            if (bounds->left + bounds->fw > bounds->w || bounds->top + bounds->fh > bounds->h) return 0;
            unsigned flags = bytes[pos + 8];
            pos += 9;
            if (flags & 128) pos += 3U << ((flags & 7) + 1);
            if (pos >= size || bytes[pos] < 2 || bytes[pos] > 8) return 0;
            pos++;
            return blocks(bytes, size, &pos);
        } else return 0;
    }
    return 0;
}
SDL_Surface *retrom_gif_load(const unsigned char *bytes, size_t size) {
    GifBounds bounds = {0};
    if (!valid_gif(bytes, size, &bounds)) return NULL;
    SDL_RWops *stream = SDL_RWFromConstMem(bytes, (int)size);
    if (!stream) return NULL;
    SDL_Surface *surface = IMG_LoadGIF_RW(stream);
    SDL_RWclose(stream);
    if (surface && (!surface->format->palette || surface->format->BytesPerPixel != 1 ||
        !dimensions((unsigned)surface->w, (unsigned)surface->h))) {
        SDL_FreeSurface(surface);
        return NULL;
    }
    if (!surface || (surface->w == (int)bounds.w && surface->h == (int)bounds.h)) return surface;
    SDL_Surface *canvas = SDL_CreateRGBSurface(0, bounds.w, bounds.h, 8, 0, 0, 0, 0);
    if (canvas) {
        SDL_SetPaletteColors(canvas->format->palette, surface->format->palette->colors, 0, surface->format->palette->ncolors);
        memset(canvas->pixels, 0, (size_t)canvas->pitch * canvas->h);
        for (int y = 0; y < surface->h; y++) {
            memcpy((unsigned char *)canvas->pixels + (bounds.top + y) * canvas->pitch + bounds.left,
                (unsigned char *)surface->pixels + y * surface->pitch, surface->w);
        }
    }
    SDL_FreeSurface(surface);
    return canvas;
}
