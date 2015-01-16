#include "layout.h"

/* PRIVATE DEFINITIONS */

typedef struct {
	Pixmap *current; /* Pointer to current pixmap */
	Pixmap *stand; /* Standing stance */
	Pixmap *shoot; /* Shooting stance */
	Pixmap *jump; /* Jumping stance */
	Pixmap *jumpshoot; /* Shooting while jumping stance */
	Pixmap *dead; /* Dead stance */
	orientation ori; /* Orientation */
} _PlayerLayout;

typedef struct {
	short type; /* Type index */
	int y,x; /* Relative position to blocks pixmap */
} _Entry;

typedef struct {
	unsigned int n; /* Number of entries */
	_Entry *entries; /* List of entries */
	Pixmap* pm[ENEMIES_N]; /* Enemies pixmaps */
} _EnemiesLayout;

typedef struct {
	unsigned int n; /* Number of entries */
	_Entry *entries; /* List of entries */
	Pixmap* pm[ITEMS_N]; /* Items pixmaps */
} _ItemsLayout;

typedef struct {
	unsigned int n; /* Number of entries */
	_Entry *entries; /* List of entries */
	Pixmap* pm[ELEMENTS_N]; /* Bullets pixmaps */
} _BulletsLayout;


/* PUBLIC DEFINITIONS */

struct _Layout {
	_PlayerLayout player; /* Player */
	_EnemiesLayout enemies; /* Enemies */
	_ItemsLayout items; /* Items */
	_BulletsLayout bullets; /* Bullets */
	Pixmap *blocks; /* Collidable terrain */
	Pixmap *scroll_bg; /* Looping background */
	Pixmap *fixed_bg; /* Non-looping terminal-sized background */
	unsigned short bg_accum; /* Accumulators */
};	


/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

/* Resets a table as NULL of size 0 freeing entries if any */
void resetEntries(_Entry **e, unsigned int *n) {
	if(e==NULL||n==NULL) return;
	if(*e!=NULL) free(*e);
	*e=NULL;
	*n=0;
}

/* Adds an entry to the table with given attributes (also increases n) */
status addEntry(short type, int y, int x, _Entry **e, unsigned int *n) {
	_Entry *temp;
	if(e==NULL||n==NULL) return ERR;
	if(*e==NULL) {
		*e=(_Entry*)malloc(sizeof(_Entry));
		if(*e==NULL) return ERR;
		(*e)->type = type;
		(*e)->y = y;
		(*e)->x = x;
		*n = 1;
	}
	else {
		temp=(_Entry*)realloc(*e,(*n+1)*sizeof(_Entry));
		if(temp==NULL) return ERR;
		*e=temp;
		((*e)+(*n))->type = type;
		((*e)+(*n))->y = y;
		((*e)+(*n))->x = x;
		(*n)++;
	}
	return OK;
}

/* Searches given entry in the table and moves it accordingly, LO_ANY can be used to ignore type */
status moveEntry(short type, int y, int x, int ystep, int xstep, _Entry *e, unsigned int n) {
	int i;
	if(e==NULL) return ERR;
	for(i=0;i<n;i++) {
		if((type==LO_ANY || e[i].type==type) && e[i].y==y && e[i].x==x) {
			e[i].y+=ystep;
			e[i].x+=xstep;
			break;
		}
	}
	return OK;
}

/* Deletes any entry in the table at given location (also decreases n if needed) */
status delEntry(int y, int x, _Entry **e, unsigned int *n) {
	int i,j;
	unsigned int new_n;
	bool deleted[*n];
	_Entry *temp;
	if(e==NULL||n==NULL) return ERR;
	if(*e==NULL||*n==0) return OK;
	for(i=0,new_n=*n;i<*n;i++) {
		if((*e)[i].y==y && (*e)[i].x==x) {
		       new_n--;
		       deleted[i]=true;
		}
		else deleted[i] = false;
	}
	if(new_n==0) {
		free(*e);
		*e = NULL;
		*n = 0;
	}
	else {
		temp=(_Entry*)malloc(new_n*sizeof(_Entry));
		if(temp==NULL) return ERR;
		for(i=0,j=0;i<*n;i++) {
			if(!deleted[i]) {
				temp[j]=(*e)[i];
				j++;
			}
		}
		free(*e);
		*e=temp;
		*n=new_n;
	}
	return OK;
}

bool match(int ah, int aw, int ay, int ax, int bh, int bw, int by, int bx) {
	if(by-ay<=ah && by-ay>=-bh && bx-ax<=aw && bx-ax>=-bw) return true;
	return false;
}


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Loads every pixmap into memory using given ColorPair*/
Layout *initLayout(level lvl, ColorPair *cp) {
	Layout *lo;
	FILE *file;
	char buf[FILE_MAX],ch;
	int i,j,starty,startx,lvl_h,lvl_w;

	if(cp==NULL) return NULL;

	/* Open common file */
	file = fopen(COMMON_FILE,"r");
	if(file==NULL) return NULL;

	/* Allocate struct Layout */
	lo = (Layout*) malloc(sizeof(Layout));
	if(lo==NULL) {
		fclose(file);
		return NULL;
	}

	/* Read and load stances */
	/** Stand **/
	if(fscanf(file,"stand=%s\n",buf)!=1) {
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->player.stand = loadPixmap(buf,cp);
	if(lo->player.stand==NULL) {
		free(lo);
		fclose(file);
		return NULL;
	}
	/** Shoot **/
	if(fscanf(file,"shoot=%s\n",buf)!=1) {
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->player.shoot = loadPixmap(buf,cp);
	if(lo->player.shoot==NULL) {
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	/** Jump **/
	if(fscanf(file,"jump=%s\n",buf)!=1) {
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->player.jump = loadPixmap(buf,cp);
	if(lo->player.jump==NULL) {
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	/** Jumpshoot **/
	if(fscanf(file,"jumpshoot=%s\n",buf)!=1) {
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->player.jumpshoot = loadPixmap(buf,cp);
	if(lo->player.jumpshoot==NULL) {
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	/** Dead **/
	if(fscanf(file,"dead=%s\n",buf)!=1) {
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->player.dead = loadPixmap(buf,cp);
	if(lo->player.dead==NULL) {
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}

	/* Read and load enemies pixmaps */
	for(i=0;i<ENEMIES_N;i++) {
		if(fscanf(file,"enemy%*d=%s\n",buf)!=1) {
			while(i>0) destroyPixmap(lo->enemies.pm[--i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
		lo->enemies.pm[i] = loadPixmap(buf,cp);
		if(lo->enemies.pm[i]==NULL) {
			while(i>0) destroyPixmap(lo->enemies.pm[--i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
	}

	/* Read and load items */
	for(i=0;i<ITEMS_N;i++) {
		if(fscanf(file,"item%*d=%s\n",buf)!=1) {
			while(i>0) destroyPixmap(lo->items.pm[--i]);
			for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
		lo->items.pm[i] = loadPixmap(buf,cp);
		if(lo->items.pm[i]==NULL) {
			while(i>0) destroyPixmap(lo->items.pm[--i]);
			for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
	}

	/* Read and load bullets */
	for(i=0;i<ELEMENTS_N;i++) {
		if(fscanf(file,"bullet%*d=%s\n",buf)!=1) {
			while(i>0) destroyPixmap(lo->bullets.pm[--i]);
			for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
			for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
		lo->bullets.pm[i] = loadPixmap(buf,cp);
		if(lo->bullets.pm[i]==NULL) {
			while(i>0) destroyPixmap(lo->bullets.pm[--i]);
			for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
			for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			fclose(file);
			return NULL;
		}
	}

	/* Done with common file */
	fclose(file);

	/* Open level file */
	switch(lvl) {
		case TUTO:
			file = fopen(TUTO_FILE,"r");
			break;
		case LVL1:
			file = fopen(LVL1_FILE,"r");
			break;
		case LVL2:
			file = fopen(LVL2_FILE,"r");
			break;
		case LVL3:
			file = fopen(LVL3_FILE,"r");
			break;
		case LVL4:
			file = fopen(LVL4_FILE,"r");
			break;
		default:
			for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
			for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
			for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
			destroyPixmap(lo->player.dead);
			destroyPixmap(lo->player.jumpshoot);
			destroyPixmap(lo->player.jump);
			destroyPixmap(lo->player.shoot);
			destroyPixmap(lo->player.stand);
			free(lo);
			return NULL;
	}
	if(file==NULL) {
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		return NULL;
	}
	
	/* Read initial coordinates */
	if(fscanf(file,"start=(%d,%d)\n",&starty,&startx)!=2) {
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}

	/* Read and load blocks */
	if(fscanf(file,"blocks=%s\n",buf)!=1) {
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->blocks = loadPixmap(buf,cp);
	if(lo->blocks==NULL) {
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}

	/* Read and load scroll_bg */
	if(fscanf(file,"scroll_bg=%s\n",buf)!=1) {
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->scroll_bg = loadPixmap(buf,cp);
	if(lo->scroll_bg==NULL) {
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}

	/* Read and load fixed_bg */
	if(fscanf(file,"fixed_bg=%s\n",buf)!=1) {
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->fixed_bg = loadPixmap(buf,cp);
	if(lo->fixed_bg==NULL) {
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}

	/* Read and load enemies */
	if(fscanf(file,"%s\n",buf)!=1 || strcmp(buf,"enemies_map:")!=0) {
		destroyPixmap(lo->fixed_bg);
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	if(getPixmapInfo(lo->blocks, &lvl_h, &lvl_w, NULL, NULL)==ERR) {
		destroyPixmap(lo->fixed_bg);
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		fclose(file);
		return NULL;
	}
	lo->enemies.entries=NULL;
	resetEntries(&lo->enemies.entries,&lo->enemies.n); 
	for(i=0;i<lvl_h;i++) {
		for(j=0;j<lvl_w;j++) {
			if(fscanf(file,"%c",&ch)!=1) {
				resetEntries(&lo->enemies.entries,&lo->enemies.n);
				destroyPixmap(lo->fixed_bg);
				destroyPixmap(lo->scroll_bg);
				destroyPixmap(lo->blocks);
				for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
				for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
				for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
				destroyPixmap(lo->player.dead);
				destroyPixmap(lo->player.jumpshoot);
				destroyPixmap(lo->player.jump);
				destroyPixmap(lo->player.shoot);
				destroyPixmap(lo->player.stand);
				free(lo);
				fclose(file);
				return NULL;
			}
			if(isalpha(ch) && (int)(ch-'A')<ENEMIES_N) {
				if(addEntry((int)(ch-'A'),i,j,&lo->enemies.entries,&lo->enemies.n)==ERR) {
					resetEntries(&lo->enemies.entries,&lo->enemies.n);
					destroyPixmap(lo->fixed_bg);
					destroyPixmap(lo->scroll_bg);
					destroyPixmap(lo->blocks);
					for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
					for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
					for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
					destroyPixmap(lo->player.dead);
					destroyPixmap(lo->player.jumpshoot);
					destroyPixmap(lo->player.jump);
					destroyPixmap(lo->player.shoot);
					destroyPixmap(lo->player.stand);
					free(lo);
					fclose(file);
					return NULL;
				}
			}
		}
		fscanf(file,"\n");
	}
	
	/* Done with level file */
	fclose(file);

	/* Initialize empty items entries */
	lo->items.entries=NULL;
	resetEntries(&lo->items.entries,&lo->items.n);

	/* Initialize empty bullets entries */
	lo->bullets.entries=NULL;
	resetEntries(&lo->bullets.entries,&lo->bullets.n);

	/* Set initial stance */
	lo->player.current = initPixmap(starty,startx);
	if(lo->player.current==NULL) {
		resetEntries(&lo->enemies.entries,&lo->enemies.n);
		destroyPixmap(lo->fixed_bg);
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		return NULL;
	}
	if(attachPixmap(lo->player.current,lo->player.stand)==ERR) {
		destroyPixmap(lo->player.current);
		resetEntries(&lo->enemies.entries,&lo->enemies.n);
		destroyPixmap(lo->fixed_bg);
		destroyPixmap(lo->scroll_bg);
		destroyPixmap(lo->blocks);
		for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
		for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
		for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
		destroyPixmap(lo->player.dead);
		destroyPixmap(lo->player.jumpshoot);
		destroyPixmap(lo->player.jump);
		destroyPixmap(lo->player.shoot);
		destroyPixmap(lo->player.stand);
		free(lo);
		return NULL;
	}

	/* Initialize layout counters to 0 */
	lo->bg_accum = 0;
	lo->player.ori = LO_DEFAULT_ORI;
	
	return lo;
}

/* Blocks functions */
status loGetBlocksSize(Layout *lo, int *h, int *w) {
	if(lo==NULL) return ERR;
	return getPixmapInfo(lo->blocks,h,w,NULL,NULL);
}

/* Fixed_bg pixmap functions */
status loGetScreenSize(Layout *lo, int *h, int *w) {
	if(lo==NULL) return ERR;
	return getPixmapInfo(lo->fixed_bg,h,w,NULL,NULL);
}

/* Player pixmap functions */
status loGetPlayerInfo(Layout *lo, int *y, int *x, orientation *ori) {
	int p_y, p_x, lvl_x;
	if(lo==NULL) return ERR;
	if(getPixmapInfo(lo->player.current,NULL,NULL,&p_y,&p_x)==ERR) return ERR;
	if(getPixmapInfo(lo->blocks,NULL,NULL,NULL,&lvl_x)==ERR) return ERR;
	if(y!=NULL) *y = p_y;
	if(x!=NULL) *x = p_x-lvl_x;
	if(ori!=NULL) *ori = lo->player.ori;
	return OK;
}
status loMovePlayer(Layout *lo, int ystep, int xstep) {
	int i;
	if(lo==NULL) return ERR;
	if(ystep!=0) {
		if(shiftPixmap(lo->player.current,ystep,0)==ERR) return ERR;
	}
	if(xstep<0) { /* Move left */
		if(lo->player.ori!=BACKWARD) lo->player.ori=BACKWARD;
		else {
			/* Move foreground blocks */
			if(shiftPixmap(lo->blocks,0,-xstep)==ERR) return ERR;
			/* Move background */
			for(i=0;i<-xstep;i++) {
				if(lo->bg_accum==LO_BG_MOD-1)
					if(scrollPixmap(lo->scroll_bg,1)==ERR) return ERR;
				lo->bg_accum++;
				lo->bg_accum %= LO_BG_MOD;
			}
		}
	}
	if(xstep>0) { /* Move right */
		if(lo->player.ori!=FORWARD) lo->player.ori=FORWARD;
		else {
			/* Move foreground blocks */
			if(shiftPixmap(lo->blocks,0,-xstep)==ERR) return ERR;
			/* Move background */
			for(i=0;i<xstep;i++) {
				if(lo->bg_accum==LO_BG_MOD-1)
					if(scrollPixmap(lo->scroll_bg,-1)==ERR) return ERR;
				lo->bg_accum++;
				lo->bg_accum %= LO_BG_MOD;
			}
		}
	}
	return OK;
}
status loSetPlayerStance(Layout *lo, stance st) {
	if(lo==NULL) return ERR;
	switch(st) {
		case STAND: if(attachPixmap(lo->player.current,lo->player.stand)==OK) break;
		case SHOOT: if(attachPixmap(lo->player.current,lo->player.shoot)==OK) break;
		case JUMP: if(attachPixmap(lo->player.current,lo->player.jump)==OK) break;
		case JUMPSHOOT: if(attachPixmap(lo->player.current,lo->player.jumpshoot)==OK) break;
		case DEAD: if(attachPixmap(lo->player.current,lo->player.dead)==OK) break;
		default: return ERR;
	}
	return OK;
}
status loSetPlayerOri(Layout *lo, orientation ori) {
	if(lo==NULL) return ERR;
	lo->player.ori = ori;
	return OK;
}

/* Enemies pixmaps functions */
status loMoveEnemy(Layout *lo, int y, int x, int ystep, int xstep) {
	if(lo==NULL) return ERR;
	return moveEntry(LO_ANY,y,x,ystep,xstep,lo->enemies.entries,lo->enemies.n);
}
status loDestroyEnemy(Layout *lo, int y, int x) {
	if(lo==NULL) return ERR;
	return delEntry(y,x,&lo->enemies.entries,&lo->enemies.n);
}

/* Items pixmaps functions */
status loCreateItem(Layout *lo, int y, int x, item_t type) {
	if(lo==NULL||type<0||type>=ITEMS_N) return ERR;
	return addEntry(type,y,x,&lo->items.entries,&lo->items.n);
}
status loMoveItem(Layout *lo, int y, int x, int ystep, int xstep) {
	if(lo==NULL) return ERR;
	return moveEntry(LO_ANY,y,x,ystep,xstep,lo->items.entries,lo->items.n);
}
status loDestroyItem(Layout *lo, int y, int x) {
	if(lo==NULL) return ERR;
	return delEntry(y,x,&lo->items.entries,&lo->items.n);

}

/* Bullets pixmaps functions */
status loCreateBullet(Layout *lo, int y, int x, element type) {
	if(lo==NULL||type<0||type>=ELEMENTS_N) return ERR;
	return addEntry(type,y,x,&lo->bullets.entries,&lo->bullets.n);
}
status loMoveBullets(Layout *lo, int ystep, int xstep, element type) {
	int i, p_x, p_w, lvl_x;
	_Entry temp;
	if(lo==NULL||type>=ELEMENTS_N) return ERR;
	if(!lo->bullets.n) return OK;
	if(getPixmapInfo(lo->player.current, NULL, &p_w, NULL, &p_x)==ERR) return ERR;
	if(getPixmapInfo(lo->blocks, NULL, NULL, NULL, &lvl_x)==ERR) return ERR;
	for(i=0;i<lo->bullets.n;i++) { /* Move all matching bullets */
		temp=lo->bullets.entries[i];
		if(temp.type==type) {
			 /* Heading right from player */
			if(temp.x>p_x+p_w/2-lvl_x) {
				if(moveEntry(type,temp.y,temp.x,ystep,xstep,lo->bullets.entries,lo->bullets.n)==ERR) return ERR;
			}
			else { /* Heading left from player */
				if(moveEntry(type,temp.y,temp.x,ystep,-xstep,lo->bullets.entries,lo->bullets.n)==ERR) return ERR;
			}
		}
	}
	return OK;
}
status loDestroyBullet(Layout *lo, int y, int x) {
	if(lo==NULL) return ERR;
	return delEntry(y,x,&lo->bullets.entries,&lo->bullets.n);
}
status loCleanBullets(Layout *lo) {
	int i, lvl_h, lvl_w;
	_Entry temp;
	if(lo==NULL) return ERR;
	if(!lo->bullets.n) return OK;
	if(getPixmapInfo(lo->blocks,&lvl_h,&lvl_w,NULL,NULL)==ERR) return ERR;
	for(i=0;i<lo->bullets.n;i++) { /* Delete all bullets out of boundaries */
		temp=lo->bullets.entries[i];
		if(temp.y<0||temp.x<0||temp.y>=lvl_h||temp.x>=lvl_w) {
			if(delEntry(temp.y,temp.x,&lo->bullets.entries,&lo->bullets.n)==ERR) return ERR;
		}
	}
	return OK;
}

/* Returns number of bullets matching enemies and stores their types and positions */
unsigned int loMatchBulletEnemy(Layout *lo, element **b_t, int **b_y, int **b_x, enemy_t **e_t, int **e_y, int **e_x) {
	element *bullets=NULL;
	int *bullets_y=NULL, *bullets_x=NULL;
	enemy_t *enemies=NULL;
	int *enemies_y=NULL, *enemies_x=NULL;
	void *more=NULL;
	int i, j, matches, b_h, b_w, e_h, e_w;
	_Entry b, e;

	if(lo==NULL) return -1;

	/* Check enemies and bullets matches */
	for(matches=0,i=0;i<lo->enemies.n;i++) {
		for(j=0;j<lo->bullets.n;j++) {
			/* Get their info */
			b = lo->bullets.entries[j];
			e = lo->enemies.entries[i];
			if(getPixmapInfo(lo->bullets.pm[b.type],&b_h,&b_w,NULL,NULL)==ERR) return ERR;
			if(getPixmapInfo(lo->enemies.pm[e.type],&e_h,&e_w,NULL,NULL)==ERR) return ERR;
			/* Check collision */
			if(match(e_h,e_w,e.y,e.x,b_h,b_w,b.y,b.x)) {

				/* Add a matching to be output... */
				matches++;

				/* ...bullet */
				more = realloc(bullets,matches*sizeof(element));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				bullets = (element*)more;
				bullets[matches-1] = b.type;

				more = realloc(bullets_y,matches*sizeof(int));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				bullets_y = (int*)more;
				bullets_y[matches-1] = b.y;

				more = realloc(bullets_x,matches*sizeof(int));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				bullets_x = (int*)more;
				bullets_x[matches-1] = b.x;

				/* ...enemy */
				more = realloc(enemies,matches*sizeof(enemy_t));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				enemies = (enemy_t*)more;
				enemies[matches-1] = e.type;

				more = realloc(enemies_y,matches*sizeof(int));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				enemies_y = (int*)more;
				enemies_y[matches-1] = e.y;

				more = realloc(enemies_x,matches*sizeof(int));
				if(more==NULL) {
					if(bullets!=NULL) free(bullets);
					if(bullets_y!=NULL) free(bullets_y);
					if(bullets_x!=NULL) free(bullets_x);
					if(enemies!=NULL) free(enemies);
					if(enemies_y!=NULL) free(enemies_y);
					if(enemies_x!=NULL) free(enemies_x);
					return -1;
				}
				enemies_x = (int*)more;
				enemies_x[matches-1] = e.x;
			}
		}
	}
	*b_t = bullets;
	*b_y = bullets_y;
	*b_x = bullets_x;
	*e_t = enemies;
	*e_y = enemies_y;
	*e_x = enemies_x;
	return matches;
}

/* Returns what is at player location:
 * -1=error, 0=nothing, 1=item, 2=enemy
 * Also stores item info */
short loMatchPlayer(Layout *lo, int p_h, int p_w, item_t *item, int *y, int *x) {
	int p_y, p_x, e_h, e_w, i;
	_Entry e;

	if(lo==NULL||item==NULL||y==NULL||x==NULL||p_h<0||p_w<0) return -1;

	if(loGetPlayerInfo(lo,&p_y,&p_x,NULL)==ERR) return -1;

	/* Check enemy collision */
	for(i=0;i<lo->enemies.n;i++) { 
		e = lo->enemies.entries[i];
		if(getPixmapInfo(lo->enemies.pm[e.type],&e_h,&e_w,NULL,NULL)==ERR) return -1;
		if(match(e_h,e_w,e.y,e.x,p_h,p_w,p_y,p_x)) return 2;
	}

	/* Check item collision */
	for(i=0;i<lo->items.n;i++) {
		e = lo->items.entries[i];
		if(getPixmapInfo(lo->items.pm[e.type],&e_h,&e_w,NULL,NULL)==ERR) return -1;
		if(match(e_h,e_w,e.y,e.x,p_h,p_w,p_y,p_x)) {
				*item = e.type;
				*x = e.x;
				*y = e.y;
				return 1;
		}
	}

	/* Nothing out there but chickens */
	return 0;
}

/* Prints layout in window using given ColorPair */
status printLayout(WINDOW *win, Layout *lo, ColorPair *cp) {
	Pixmap *temp,*old;
	int i, j, lvl_h, lvl_w, lvl_x;
	_Entry e;

	if(win==NULL||lo==NULL||cp==NULL) return ERR;

	/* temp = fixed_bg + scroll_bg */
	temp = mergePixmaps(lo->scroll_bg,lo->fixed_bg,cp,false);
	if(temp==NULL) return ERR;
	old=temp;

	/* temp += blocks */
	temp = mergePixmaps(lo->blocks,temp,cp,false);
	destroyPixmap(old);
	if(temp==NULL) return ERR;
	old=temp;

	/* Get background info for loops */
	if(getPixmapInfo(lo->blocks, &lvl_h, &lvl_w, NULL, &lvl_x)==ERR) {
		destroyPixmap(old);
		return ERR;
	}

	/* temp += items */
	for(i=0;i<lo->items.n;i++) {
		e = lo->items.entries[i];
		if(movePixmap(lo->items.pm[e.type],e.y,e.x+lvl_x)==ERR) {
			destroyPixmap(old);
			return ERR;
		}
		temp = mergePixmaps(lo->items.pm[e.type],temp,cp,false);
		destroyPixmap(old);
		if(temp==NULL) return ERR;
		old=temp;
	}

	/* temp += bullets */
	for(i=0;i<lo->bullets.n;i++) {
		e = lo->bullets.entries[i];
		if(movePixmap(lo->bullets.pm[e.type],e.y,e.x+lvl_x)==ERR) {
			destroyPixmap(old);
			return ERR;
		}
		temp = mergePixmaps(lo->bullets.pm[e.type],temp,cp,false);
		destroyPixmap(old);
		if(temp==NULL) return ERR;
		old=temp;
	}

	/* temp += enemies */
	for(i=0;i<lo->enemies.n;i++) {
		e = lo->enemies.entries[i];
		if(movePixmap(lo->enemies.pm[e.type],e.y,e.x+lvl_x)==ERR) {
			destroyPixmap(old);
			return ERR;
		}
		temp = mergePixmaps(lo->enemies.pm[e.type],temp,cp,false);
		destroyPixmap(old);
		if(temp==NULL) return ERR;
		old=temp;
	}

	/* temp += player */
	temp = mergePixmaps(lo->player.current,temp,cp,lo->player.ori);
	destroyPixmap(old);
	if (temp==NULL) return ERR;

	if(printPixmap(win,temp,false)==ERR) {
		destroyPixmap(temp);
		return ERR;
	}

	destroyPixmap(temp);
	return OK;
}

/* Frees memory of given layout */
void destroyLayout(Layout *lo) {
	int i;
	if(lo==NULL) return;
	detachPixmap(lo->player.current);
	destroyPixmap(lo->player.current);
	destroyPixmap(lo->fixed_bg);
	resetEntries(&lo->bullets.entries,&lo->bullets.n);
	resetEntries(&lo->items.entries,&lo->items.n);
	resetEntries(&lo->enemies.entries,&lo->enemies.n);
	destroyPixmap(lo->scroll_bg);
	destroyPixmap(lo->blocks);
	for(i=0;i<ELEMENTS_N;i++) destroyPixmap(lo->bullets.pm[i]);
	for(i=0;i<ITEMS_N;i++) destroyPixmap(lo->items.pm[i]);
	for(i=0;i<ENEMIES_N;i++) destroyPixmap(lo->enemies.pm[i]);
	destroyPixmap(lo->player.dead);
	destroyPixmap(lo->player.jumpshoot);
	destroyPixmap(lo->player.jump);
	destroyPixmap(lo->player.shoot);
	destroyPixmap(lo->player.stand);
	free(lo);
}
