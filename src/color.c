#include "color.h"

/* PRIVATE DEFINITIONS */

typedef struct {
	bool space; /* Whether the block is full */
	bool solid[2]; /* Solid/Transparency. Index 0 means top pixel, 1 means bottom */
	short cp; /* ColorPair index used to print the character */
} _DPix;


/* PUBLIC DEFINITIONS */

struct _ColorPair {
	short fg; /* Foreground color */
	short bg; /* Background color */
};

struct _Pixmap {
	int height, width; /* Size */
	int starty, startx; /* Starting (relative) position */
	int y, x; /* Current (relative) position */
	_DPix **map; /* Map of DPixels forming the tile */
};


/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

void swap(_DPix *pix1, _DPix *pix2) {
	short temp;
	if(pix1==NULL || pix2==NULL) return;
	if(pix1->space!=pix2->space) {
		temp = pix1->space;
		pix1->space = pix2->space;
		pix2->space = temp;
	}
	if(pix1->solid[0]!=pix2->solid[0]) {
		temp = pix1->solid[0];
		pix1->solid[0] = pix2->solid[0];
		pix2->solid[0] = temp;
	}
	if(pix1->solid[1]!=pix2->solid[1]) {
		temp = pix1->solid[1];
		pix1->solid[1] = pix2->solid[1];
		pix2->solid[1] = temp;
	}
	if(pix1->cp!=pix2->cp) {
		temp = pix1->cp;
		pix1->cp = pix2->cp;
		pix2->cp = temp;
	}
}


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/** ColorPair Functions **/

/* Initializes all possible needed color pairs into a given array and loads them into ncurses */
ColorPair *initColorPairs() {
	ColorPair *cp;
	short i,j,k;
	
	/* Allocate array for color pairs */;
	cp = (ColorPair*) malloc(NCOLORPAIRS*sizeof(ColorPair));
	if(cp==NULL) return NULL;

	/* 2-combinations with repetition of colors, excluding matching pairs */
	for(i=0,k=0;i<NCOLORS;i++) {
		for(j=i+1;j<NCOLORS;j++) {
			/* Initialize (i,j) */
			cp[k].fg = i;
			cp[k].bg = j;
			init_pair(++k,i,j);
			/* Initialize (j,i) */
			cp[k].fg = j;
			cp[k].bg = i;
			init_pair(++k,j,i);
		}
	}

	return cp;
}

/* Given top and bottom colors and an array of color pairs, looks for the matching
   ColorPair into the array and returns its index, or -1 in case of error */
short findColorPair(ColorPair *cp, short top, short bot) {
	short i;

	if(cp==NULL || top<0 || bot<0 || top>NCOLORS-1 || bot>NCOLORS-1) return -1;
	for(i=0;i<NCOLORPAIRS;i++) {
		/* Entirely bot=top-colored space */
		if(top==bot && cp[i].bg==top) return i;
		/* Bot-colored lower half block over top-colored background */
		if(cp[i].fg==bot && cp[i].bg==top) return i;
	}

	return -1; /* Not found (Unlikely) */
}

/* Frees a given array of color pairs */
void destroyColorPairs(ColorPair *cp) {
	if(cp==NULL) return;
	free(cp);
}


/** Pixmap Functions **/

/* Initializes a pixmap with given values and zero size */
Pixmap *initPixmap(int y, int x) {
	Pixmap *pm;
	pm = (Pixmap*) malloc(sizeof(Pixmap));
	if(pm==NULL) return NULL;
	pm->height = pm->width = 0;
	pm->starty = pm->y = y;
	pm->startx = pm->x = x;
	pm->map = NULL;
	return pm;
}

/* Loads a pixmap structure from a file using a given array of color pairs */
Pixmap *loadPixmap(char *file, ColorPair *cp) {
	Pixmap *pm;
	FILE *source;
	int y, x;
	short top,bot;
	int solid[2];

	if(cp==NULL || file==NULL) return NULL;

	source = fopen(file,"r");
	if(source==NULL) return NULL;

	pm = (Pixmap*) malloc(sizeof(Pixmap));
	if(pm==NULL) {
		fclose(source);
		return NULL;
	}

	/* Scan size and starting position */
	if(fscanf(source, "height=%d;\n", &(pm->height)) != 1) {
		free(pm);
		fclose(source);
		return NULL;
	}
	if(fscanf(source, "width=%d;\n", &(pm->width)) != 1) {
		free(pm);
		fclose(source);
		return NULL;
	}
	if(fscanf(source, "starty=%d;\n", &(pm->starty)) != 1) {
		free(pm);
		fclose(source);
		return NULL;
	}
	if(fscanf(source, "startx=%d;\n", &(pm->startx)) != 1) {
		free(pm);
		fclose(source);
		return NULL;
	}
	/* Check that size is valid */
	if(!(pm->height>0) || !(pm->width>0)) {
		free(pm);
		fclose(source);
		return NULL;
	}
	/* Initialize current position */
	pm->y = pm->starty;
	pm->x = pm->startx;

	/* Allocate double array of DPixels */
	pm->map = (_DPix**) malloc(pm->height*sizeof(_DPix*));
	if(pm->map==NULL) {
		free(pm);
		fclose(source);
		return NULL;
	}
	/* Go through it */
	for(y=0;y<pm->height;y++) {
		pm->map[y] = (_DPix*) malloc(pm->width*sizeof(_DPix));
		if(pm->map[y]==NULL) {
			while(y>0) free(pm->map[--y]);
			free(pm);
			fclose(source);
			return NULL;
		}
		for(x=0;x<pm->width;x++) {
			/* Scan colors and transparency of top and bottom pixels */
			if(fscanf(source, "%hd,%hd:%d,%d", &top, &bot, &solid[0], &solid[1])!=4) {
				while(y>0) free(pm->map[--y]);
				free(pm);
				fclose(source);
				return NULL;
			}
			/* Set DPix attributes according to read information */
			pm->map[y][x].cp = findColorPair(cp,top,bot);
			if(top==bot) pm->map[y][x].space = true;
			else pm->map[y][x].space = false;
			pm->map[y][x].solid[0] = solid[0];
			pm->map[y][x].solid[1] = solid[1];
			if(pm->map[y][x].cp==-1) {
				while(y>0) free(pm->map[--y]);
				free(pm);
				fclose(source);
				return NULL;
			}

		}
	}

	fclose(source);

	return pm;
}

/* Writes a pixmap structure to a file using a given array of color pairs */
status savePixmap(Pixmap *pm, ColorPair *cp, char *file) {
	FILE *output;
	int y,x;
	short top,bot;
	bool solid[2];

	if(pm==NULL || cp==NULL || file==NULL) return ERR;

	output = fopen(file,"w");
	if(output==NULL) return ERR;
	
	/* Write size and starting position */
	if(fprintf(output,"height=%d;\n",pm->height)<0) {
		fclose(output);
		return ERR;
	}
	if(fprintf(output,"width=%d;\n",pm->width)<0) {
		fclose(output);
		return ERR;
	}
	if(fprintf(output,"starty=%d;\n",pm->starty)<0) {
		fclose(output);
		return ERR;
	}
	if(fprintf(output,"startx=%d;\n",pm->startx)<0) {
		fclose(output);
		return ERR;
	}

	/* Go through double array of DPixels */
	for(y=0;y<pm->height;y++) {
		for(x=0;x<pm->width;x++) {
			/* Retrieve color attributes */
			if(pm->map[y][x].space == true)
				/* Space, top and bottom pixels are same color */
				top = bot = cp[pm->map[y][x].cp].bg;
			else {
				/* Lower half block, top color is bg and bot is fg */
				top = cp[pm->map[y][x].cp].bg;
				bot = cp[pm->map[y][x].cp].fg;
			}
			/* Retrieve transparency attributes and print DPix info*/
			solid[0] = pm->map[y][x].solid[0];
			solid[1] = pm->map[y][x].solid[1];
			fprintf(output,"%hd,%hd:%d,%d ",top,bot,solid[0],solid[1]);
		}
		/* Width reached: next line */
		fprintf(output,"\n");
	}

	fclose(output);

	return OK;
}

/* Stores pixmap info in given addresses (if not NULL) */
status getPixmapInfo(Pixmap *pm, int *height, int *width, int *y, int *x) {
	if(pm==NULL) return ERR;
	if(height!=NULL) *height = pm->height;
	if(width!=NULL) *width = pm->width;
	if(y!=NULL) *y = pm->y;
	if(x!=NULL) *x = pm->x;
	return OK;
}

/* Scrolls a pixmap horizontally a given amount of steps */
status scrollPixmap(Pixmap *pm, int step) {
	int i,y,x;

	if(pm==NULL) return ERR;

	if(step<0) {
		for(i=0;i>step;i--)
			for(y=0;y<pm->height;y++)
				for(x=1;x<pm->width;x++)
					swap(&(pm->map[y][x-1]),&(pm->map[y][x]));
	}
	else {
		for(i=0;i<step;i++)
			for(y=0;y<pm->height;y++)
				for(x=pm->width-1;x>0;x--)
					swap(&(pm->map[y][x-1]),&(pm->map[y][x]));
	}
	return OK;
}

/* Sets given pixmap position to (y,x) */
status movePixmap(Pixmap *pm, int y, int x) {
	if(pm==NULL) return ERR;
	pm->y = y;
	pm->x = x;
	return OK;
}

/* Shifts given pixmap position using vector (stepy,stepx) */
status shiftPixmap(Pixmap *pm, int stepy, int stepx) {
	if(pm==NULL) return ERR;
	pm->y += stepy;
	pm->x += stepx;
	return OK;
}

/* Resets given pixmap to its starting position */
status resetPixmap(Pixmap *pm) {
	if(pm==NULL) return ERR;
	pm->y = pm->starty;
	pm->x = pm->startx;
	return OK;
}

/* Attachs map pointer and copies size from src to dest pixmap */
status attachPixmap(Pixmap *dest, Pixmap *src) {
	if(dest==NULL||src==NULL) return ERR;
	dest->height = src->height;
	dest->width = src->width;
	dest->map = src->map;
	return OK;
}

/* Prints into a window (usually stdscr) a given pixmap at its current (relative) position */
status printPixmap(WINDOW *win, Pixmap *pm, bool refresh) {
	int y, x;
	if(win==NULL || pm==NULL) return ERR;

	/* Go trough the double array of DPixels */
	for(y=0;y<pm->height;y++) {
		for(x=0;x<pm->width;x++) {
			/* NOTE: ncurses' array for COLOR_PAIR starts from index 1 while
			   implemented ColorPair array starts from 0 */
			if(pm->map[y][x].space == true) {
				/* Space */
				wattron(win,COLOR_PAIR((pm->map[y][x].cp)+1));
				mvwprintw(win,(pm->y)+y,(pm->x)+x," ");
				wattroff(win,COLOR_PAIR((pm->map[y][x].cp)+1));
			} else {
				/* Lower Half Block */
				wattron(win,COLOR_PAIR((pm->map[y][x].cp)+1));
				mvwprintw(win,(pm->y)+y,(pm->x)+x,"\u2584");
				wattroff(win,COLOR_PAIR((pm->map[y][x].cp)+1));
			}
		}
	}
	
	if(refresh)
		refresh(); /* Refresh terminal screen to make changes visible */

	return OK;
}

/* Prints a foreground pixmap into a background pixmap at its current (relative) position using
   transparency and returns the resulting pixmap. Foreground can be mirrored */
Pixmap *mergePixmaps(Pixmap *fg, Pixmap *bg, ColorPair *cp, bool mirror) {
	Pixmap *pm;
	int y,x;
	short top,bot;

	if(fg==NULL || bg==NULL || cp==NULL) return NULL;

	pm = (Pixmap*) malloc(sizeof(Pixmap));
	if(pm==NULL) return NULL;

	/* Use size, position and starting position from background pixmap */
	pm->height = bg->height;
	pm->width = bg->width;
	pm->y = bg->y;
	pm->x = bg->x;
	pm->starty = bg->starty;
	pm->startx = bg->startx;

	/* Allocate double array of DPixels */
	pm->map = (_DPix**) malloc(pm->height*sizeof(_DPix*));
	if(pm->map==NULL) {
		free(pm);
		return NULL;
	}
	for(y=0;y<pm->height;y++) {
		pm->map[y] = (_DPix*) malloc(pm->width*sizeof(_DPix));
		if(pm->map[y]==NULL) {
			while(y>0) free(pm->map[--y]);
			free(pm);
			return NULL;
		}
		for(x=0;x<pm->width;x++) {
			/* Use background DPixels for now */
			pm->map[y][x].space=bg->map[y][x].space;
			pm->map[y][x].solid[0]=bg->map[y][x].solid[0];
			pm->map[y][x].solid[1]=bg->map[y][x].solid[1];
			pm->map[y][x].cp=bg->map[y][x].cp;
		}
	}

	/* BEWARE: This is the most fucking counterintuitive loop I've ever made: your head may burst into flames! */
	/* Go through overlapped area between background and foreground pixmaps until any of them ends */
	for(y=fg->y;y<(fg->y+fg->height)&&y<bg->height;y++) {
		if(y>=0 && y<bg->height) {
			for(x=fg->x;x<(fg->x+fg->width)&&y<bg->width;x++) {
				if(x>=0 && x<bg->width) {
					switch(mirror) {
						case false:
							/* Transparent foreground block */
							if(fg->map[y-(fg->y)][x-(fg->x)].space==true &&
							    fg->map[y-(fg->y)][x-(fg->x)].solid[0]==false &&
							    fg->map[y-(fg->y)][x-(fg->x)].solid[1]==false);
								/* Do nothing. Use background DPix, which is already copied */

							/* Solid foreground block */
							else if(fg->map[y-(fg->y)][x-(fg->x)].space==true &&
							    fg->map[y-(fg->y)][x-(fg->x)].solid[0]==true &&
							    fg->map[y-(fg->y)][x-(fg->x)].solid[1]==true) {
								/* Use foreground DPix */
								pm->map[y][x].space=fg->map[y-(fg->y)][x-(fg->x)].space;
								pm->map[y][x].solid[0]=fg->map[y-(fg->y)][x-(fg->x)].solid[0];
								pm->map[y][x].solid[1]=fg->map[y-(fg->y)][x-(fg->x)].solid[1];
								pm->map[y][x].cp=fg->map[y-(fg->y)][x-(fg->x)].cp;
							}
							/* Not an uniform block */
							else {
								/* Check transparency of top pixel */
								if(fg->map[y-(fg->y)][x-(fg->x)].solid[0]==true)
									/* Solid: use foreground pixmap top color */
									top = cp[fg->map[y-(fg->y)][x-(fg->x)].cp].bg;
								else
									/* Transparent: use background pixmap top color */
									top = cp[bg->map[y][x].cp].bg;

								/* Check transparency of bottom pixel */
								if(fg->map[y-(fg->y)][x-(fg->x)].solid[1]==true)
									/* Solid: use foreground pixmap bottom color */

									/* Space: use top as bottom color */
									if(fg->map[y-(fg->y)][x-(fg->x)].space==true) 
										bot = cp[fg->map[y-(fg->y)][x-(fg->x)].cp].bg;
									/* Not a space: use bottom color normally */
									else
										bot = cp[fg->map[y-(fg->y)][x-(fg->x)].cp].fg;
								else {
									/* Transparent: use background pixmap bottom color */
									if(bg->map[y][x].space==true)
										/* Space: bottom color is bg */
										bot = cp[bg->map[y][x].cp].bg;
									else
										/* Lower half block: bottom color is fg */
										bot = cp[bg->map[y][x].cp].fg;
								}

								/* Set DPix attributes according to information fetched */
								pm->map[y][x].cp=findColorPair(cp,top,bot);
								if(top==bot) pm->map[y][x].space = true;
								else pm->map[y][x].space = false;
							}
							break;
						case true:
							/* Transparent foreground block */
							if(fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].space==true &&
							    fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[0]==false &&
							    fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[1]==false);
								/* Do nothing. Use background DPix, which is already copied */

							/* Solid foreground block */
							else if(fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].space==true &&
							    fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[0]==true &&
							    fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[1]==true) {
								/* Use foreground DPix */
								pm->map[y][x].space=fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].space;
								pm->map[y][x].solid[0]=fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[0];
								pm->map[y][x].solid[1]=fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[1];
								pm->map[y][x].cp=fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].cp;
							}
							/* Not a block */
							else {
								/* Check transparency of top pixel */
								if(fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[0]==true)
									/* Solid: use foreground pixmap top color */
									top = cp[fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].cp].bg;
								else
									/* Transparent: use background pixmap top color */
									top = cp[bg->map[y][x].cp].bg;

								/* Check transparency of bottom pixel */
								if(fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].solid[1]==true)
									/* Solid: use foreground pixmap bottom color */
									bot = cp[fg->map[y-(fg->y)][(fg->x)-x+(fg->width-1)].cp].fg;
								else {
									/* Transparent: use background pixmap bottom color */
									if(bg->map[y][x].space==true)
										/* Space: bottom color is bg */
										bot = cp[bg->map[y][x].cp].bg;
									else
										/* Lower half block: bottom color is fg */
										bot = cp[bg->map[y][x].cp].fg;
								}

								/* Set DPix attributes according to information fetched */
								pm->map[y][x].cp=findColorPair(cp,top,bot);
								if(top==bot) pm->map[y][x].space = true;
								else pm->map[y][x].space = false;
							}
							break;
					}
				}
			}
		}
	}

	return pm;
}

/* Detaches map of given pixmap */
void detachPixmap(Pixmap *pm) {
	if(pm==NULL) return;
	pm->map = NULL;
}

/* Frees a given pixmap from memory */
void destroyPixmap(Pixmap *pm) {
	int i;
	if(pm==NULL) return;
	if(pm->map!=NULL) {
		for(i=pm->height-1;i>=0;i--) free(pm->map[i]);
		free(pm->map);
	}
	free(pm);
}
