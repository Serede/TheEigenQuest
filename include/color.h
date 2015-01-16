#ifndef _COLOR_H
#define _COLOR_H

#include "types.h"

#define NCOLORS 8
#define NCOLORPAIRS 56

typedef struct _ColorPair ColorPair;
typedef struct _Pixmap Pixmap;

/** ColorPair Functions **/

/* Initializes all possible needed color pairs into a given array and loads them into ncurses */
ColorPair *initColorPairs();

/* Given top and bottom colors and an array of color pairs, looks for the matching ColorPair
   into the array and returns its index, or -1 in case of error */
short findColorPair(ColorPair *cp, short top, short bot);

/* Frees a given array of color pairs */
void destroyColorPairs(ColorPair *cp);


/** Pixmap Functions **/

/* Initializes a pixmap with given values and zero size */
Pixmap *initPixmap(int y, int x);

/* Loads a pixmap structure from a file using a given array of color pairs */
Pixmap *loadPixmap(char *file, ColorPair *cp);

/* Writes a pixmap structure to a file using a given array of color pairs */
status savePixmap(Pixmap *pm, ColorPair *cp, char *file);

/* Stores pixmap info in given addresses (if not NULL) */
status getPixmapInfo(Pixmap *pm, int *height, int *width, int *y, int *x);

/* Scrolls a pixmap horizontally a given amount of steps */
status scrollPixmap(Pixmap *pm, int step);

/* Sets given pixmap position to (y,x) */
status movePixmap(Pixmap *pm, int y, int x);

/* Shifts given pixmap position using vector (stepy,stepx) */
status shiftPixmap(Pixmap *pm, int stepy, int stepx);

/* Resets given pixmap to its starting position */
status resetPixmap(Pixmap *pm);

/* Attach map pointer and copies size from src to dest pixmap */
status attachPixmap(Pixmap *dest, Pixmap *src);

/* Prints into a window (usually stdscr) a given pixmap at its current (relative) position */
status printPixmap(WINDOW *win, Pixmap *pm, bool refresh);

/* Prints a foreground pixmap into a background pixmap at its current (relative) position using
   transparency and returns the resulting pixmap. Foreground can be mirrored */
Pixmap *mergePixmaps(Pixmap *fg, Pixmap *bg, ColorPair *cp, bool mirror);

/* Detaches map of given pixmap */
void detachPixmap(Pixmap *pm);

/* Frees a given Pixmap from memory */
void destroyPixmap(Pixmap *pm);

#endif
