#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>

int get_fps();


/*
typedef struct {
	uint32_t flags;
	SDL_PixelFormat *format;
	int w, h; // Width and height of the surface
	uint16_t pitch; // Length of a surface scanline in bytes
	SDL_Rect clip_rect; // surface clip rectangle

	int refcount;
	uint8_t *pixels; // Pointer to the actual pixel data

} SDL_Surface;

typedef struct {
	int16_t x, y; // Position of the upper-left corner of the rectangle
	uint16_t w, h; // The width and height of the rectangle
} SDL_Rect;
*/
// src the SDL_Surface structure to be copied from
// srcrect the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
// dst the SDL_Surface structure that is the blit target
// dstrect the SDL_Rect structure representing the rectangle that is copied into
void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(dst && src);

	int sx = (srcrect == NULL ? 0 : srcrect->x);
	int sy = (srcrect == NULL ? 0 : srcrect->y);
	int dx = (dstrect == NULL ? 0 : dstrect->x);
	int dy = (dstrect == NULL ? 0 : dstrect->y);
	int w = (srcrect == NULL ? src->w : srcrect->w);
	int h = (srcrect == NULL ? src->h : srcrect->h);
	// 目标屏幕的宽度小于要复制的矩形
	if(dst->w - dx < w) { w = dst->w - dx; }
	if(dst->h - dy < h) { h = dst->h - dy; }
	if(dstrect != NULL) {
		dstrect->w = w;
		dstrect->h = h;
	}

	/* TODO: copy pixels from position (`sx', `sy') with size
	 * `w' X `h' of `src' surface to position (`dx', `dy') of
	 * `dst' surface.
	 */
	int i, j;
	for(i = 0; i < w; i++){
		for(j = 0; j < h; j++){
			dst->pixels[dx + i + dst->pitch * (dy + j)] = src->pixels[sx + i + src->pitch * (sy + j)];
		}
	}
	// assert(0);
}

// Use this function to perform a fast fill of a rectangle with a specific color.
void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	assert(dst);
	assert(color <= 0xff);

	/* TODO: Fill the rectangle area described by `dstrect'
	 * in surface `dst' with color `color'. If dstrect is
	 * NULL, fill the whole surface.
	 */
	int dx = (dstrect == NULL ? 0 : dstrect->x);
	int dy = (dstrect == NULL ? 0 : dstrect->y);
	int w = (dstrect == NULL ? dst->w : dstrect->w);
	int h = (dstrect == NULL ? dst->h : dstrect->h);
	int i, j;
	for(i = 0; i < w; i++){
		for(j = 0; j < h; j++){
			dst->pixels[dx + i + dst->pitch * (dy + j)] = color;
		}
	}
	// assert(0);
}


// SDL_SetPalette - Sets the colors in the palette of an 8-bit surface.
// Sets a portion of the palette for the given 8-bit surface.
void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, 
		int firstcolor, int ncolors) {
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	if(s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors) {
		if(s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL) {
			/* If the size of the new palette is different 
			 * from the old one, free the old one.
			 */
			free(s->format->palette->colors);
		}

		/* Get new memory space to store the new palette. */
		s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
		assert(s->format->palette->colors);
	}

	/* Set the new palette. */
	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if(s->flags & SDL_HWSURFACE) {
		/* TODO: Set the VGA palette by calling write_palette(). */
		write_palette(colors, ncolors);
		// assert(0);
	}
}

/* ======== The following functions are already implemented. ======== */

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h) {
	assert(screen);
	assert(screen->pitch == 320);

	// this should always be true in NEMU-PAL
	assert(screen->flags & SDL_HWSURFACE);

	if(x == 0 && y == 0) {
		/* Draw FPS */
		vmem = VMEM_ADDR;
		char buf[80];
		sprintf(buf, "%dFPS", get_fps());
		draw_string(buf, 0, 0, 10);
	}
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(src && dst);
	int x = (srcrect == NULL ? 0 : srcrect->x);
	int y = (srcrect == NULL ? 0 : srcrect->y);
	int w = (srcrect == NULL ? src->w : srcrect->w);
	int h = (srcrect == NULL ? src->h : srcrect->h);

	assert(dstrect);
	if(w == dstrect->w && h == dstrect->h) {
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else {
		/* No other case occurs in NEMU-PAL. */
		assert(0);
	}
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
		uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s);
	s->format->palette = malloc(sizeof(SDL_Palette));
	assert(s->format->palette);
	s->format->palette->colors = NULL;

	s->format->BitsPerPixel = depth;

	s->flags = flags;
	s->w = width;
	s->h = height;
	s->pitch = (width * depth) >> 3;
	s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
	assert(s->pixels);

	return s;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
	return SDL_CreateRGBSurface(flags,  width, height, bpp,
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s) {
	if(s != NULL) {
		if(s->format != NULL) {
			if(s->format->palette != NULL) {
				if(s->format->palette->colors != NULL) {
					free(s->format->palette->colors);
				}
				free(s->format->palette);
			}

			free(s->format);
		}
		
		if(s->pixels != NULL && s->pixels != VMEM_ADDR) {
			free(s->pixels);
		}

		free(s);
	}
}

