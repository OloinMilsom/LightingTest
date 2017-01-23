#include "PolyDraw.h"
#include <algorithm>

int _gfxPrimitivesCompareInt(const void *a, const void *b) {
	return (*(const int *)a) - (*(const int *)b);
}

/* ---- Textured Polygon */

/*!
\brief Internal function to draw a textured horizontal line.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param texture The texture to retrieve color information from.
\param texture_w The width of the texture.
\param texture_h The height of the texture.
\param texture_dx The X offset for the texture lookup.
\param texture_dy The Y offset for the textured lookup.

\returns Returns 0 on success, -1 on failure.
*/
int _HLineTextured(SDL_Surface *dest, Sint16 x1, Sint16 x2, Sint16 y, SDL_Surface * source, int texture_w, int texture_h, int texture_dx, int texture_dy) {
	Sint16 w;
	Sint16 xtmp;
	int result = 0;
	int texture_x_walker;
	int texture_y_start;
	SDL_Rect source_rect, dst_rect;
	int pixels_written, write_width;

	/*
	* Swap x1, x2 if required to ensure x1<=x2
	*/
	if (x1 > x2) {
		xtmp = x1;
		x1 = x2;
		x2 = xtmp;
	}

	/*
	* Calculate width to draw
	*/
	w = x2 - x1 + 1;

	/*
	* Determine where in the texture we start drawing
	*/
	texture_x_walker = (x1 - texture_dx) % texture_w;
	if (texture_x_walker < 0) {
		texture_x_walker = texture_w + texture_x_walker;
	}

	texture_y_start = (y + texture_dy) % texture_h;
	if (texture_y_start < 0) {
		texture_y_start = texture_h + texture_y_start;
	}

	// setup the source rectangle; we are only drawing one horizontal line
	source_rect.y = texture_y_start;
	source_rect.x = texture_x_walker;
	source_rect.h = 1;

	// we will draw to the current y
	dst_rect.y = y;
	dst_rect.h = 1;

	// if there are enough pixels left in the current row of the texture
	// draw it all at once
	if (w <= texture_w - texture_x_walker) {
		source_rect.w = w;
		source_rect.x = texture_x_walker;
		dst_rect.x = x1;
		dst_rect.w = source_rect.w;
		result = (SDL_BlitSurface(source, &source_rect, dest, &dst_rect) == 0);
	}
	else {
		// we need to draw multiple times
		// draw the first segment
		pixels_written = texture_w - texture_x_walker;
		source_rect.w = pixels_written;
		source_rect.x = texture_x_walker;
		dst_rect.x = x1;
		dst_rect.w = source_rect.w;
		result |= (SDL_BlitSurface(source, &source_rect, dest, &dst_rect) == 0);
		write_width = texture_w;

		// now draw the rest
		// set the source x to 0
		source_rect.x = 0;
		while (pixels_written < w) {
			if (write_width >= w - pixels_written) {
				write_width = w - pixels_written;
			}
			source_rect.w = write_width;
			dst_rect.x = x1 + pixels_written;
			dst_rect.w = source_rect.w;
			result |= (SDL_BlitSurface(source, &source_rect, dest, &dst_rect) == 0);
			pixels_written += write_width;
		}
	}

	return result;
}

/*!
\brief Draws a polygon filled with the given texture (Multi-Threading Capable).

\param renderer The renderer to draw on.
\param vx array of x vector components
\param vy array of x vector components
\param n the amount of vectors in the vx and vy array
\param texture the sdl surface to use to fill the polygon
\param texture_dx the offset of the texture relative to the screeen. If you move the polygon 10 pixels
to the left and want the texture to apear the same you need to increase the texture_dx value
\param texture_dy see texture_dx
\param polyInts Preallocated temp array storage for vertex sorting (used for multi-threaded operation)
\param polyAllocated Flag indicating oif the temp array was allocated (used for multi-threaded operation)

\returns Returns 0 on success, -1 on failure.
*/
int texturedPolygon(SDL_Surface * dest, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface * texture, int texture_dx, int texture_dy, SDL_BlendMode blendMode) {
	int result;
	int i;
	int y, xa, xb;
	int minx, maxx, miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int gfxPrimitivesPolyAllocated = 0;
	SDL_Texture *textureAsTexture = NULL;

	/*
	* Sanity check number of edges
	*/
	if (n < 3) {
		return -1;
	}

	/*
	* Allocate temp array, only grow array
	*/
	if (!gfxPrimitivesPolyAllocated) {
		gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	}
	else {
		if (gfxPrimitivesPolyAllocated < n) {
			gfxPrimitivesPolyInts = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			gfxPrimitivesPolyAllocated = n;
		}
	}

	/*
	* Check temp array
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		gfxPrimitivesPolyAllocated = 0;
	}

	/*
	* Check temp array again
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		return(-1);
	}

	/*
	* Determine X,Y minima,maxima
	*/
	miny = vy[0];
	maxy = vy[0];
	minx = vx[0];
	maxx = vx[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		}
		else if (vy[i] > maxy) {
			maxy = vy[i];
		}
		if (vx[i] < minx) {
			minx = vx[i];
		}
		else if (vx[i] > maxx) {
			maxx = vx[i];
		}
	}

	SDL_SetSurfaceBlendMode(texture, blendMode);

	/*
	* Draw, scanning y
	*/
	result = 0;
	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			}
			else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else {
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2))) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= _HLineTextured(dest, xa, xb, y, texture, texture->w, texture->h, texture_dx, texture_dy);
		}
	}

	return (result);
}

