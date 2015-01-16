#include "physics.h"

/* PUBLIC DEFINITIONS */

struct _CollisionMap{
	int height, width; /* Size */
	bool** map; /* Map itself */
};


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/** Collision Functions **/

/* Load CollisionMap from file */
CollisionMap *loadCollisionMap(char *file) {
	CollisionMap *cm;
	FILE *source;
	int y, x;
	char collide;

	if(file==NULL) return NULL;

	source = fopen(file,"r");
	if(source==NULL) return NULL;

	cm = (CollisionMap*) malloc(sizeof(CollisionMap));
	if(cm==NULL) {
		fclose(source);
		return NULL;
	}
	/* Scan size */
	if(fscanf(source, "height=%d;\n", &cm->height) != 1) {
		free(cm);
		fclose(source);
		return NULL;
	}
	if(fscanf(source, "width=%d;\n", &cm->width) != 1) {
		free(cm);
		fclose(source);
		return NULL;
	}
	/* Check that size is valid */
	if(!(cm->height>0) || !(cm->width>0)) {
		free(cm);
		fclose(source);
		return NULL;
	}
	/* Allocate double array of bools */
	cm->map = (bool**) malloc(cm->height*sizeof(bool*));
	if(cm->map==NULL) {
		free(cm);
		fclose(source);
		return NULL;
	}
	/* Go through CollisionMap */
	for(y=0;y<cm->height;y++) {
		cm->map[y] = (bool*) malloc(cm->width*sizeof(bool));
		if(cm->map[y]==NULL) {
			while(y>0) free(cm->map[--y]);
			free(cm);
			fclose(source);
			return NULL;
		}
		for(x=0;x<cm->width;x++) {
			/* Scan each collide */
			if(fscanf(source, "%c", &collide)!=1) {
				while(y>0) free(cm->map[--y]);
				free(cm);
				fclose(source);
				return NULL;
			}
			/* Set each collide */
			if(collide=='1') cm->map[y][x] = true;
			else if(collide=='0') cm->map[y][x] = false;
			else {
				while(y>0) free(cm->map[--y]);
				free(cm);
				fclose(source);
				return NULL;
			}

		}
		fscanf(source, "\n");
	}
	fclose(source);
	return cm;
}

/* Returns whether input coordinates collide */
bool isColliding(CollisionMap *cm, int y, int x) {
	if(cm==NULL) return false;
	if(y<0||y>cm->height-1) return false;
	if(x<0||x>cm->width-1) return true;
	return cm->map[y][x];
}

/* Frees a given CollisionMap */
void destroyCollisionMap(CollisionMap *cm) {
	int i;
	if(cm==NULL) return;
	for(i=cm->height-1;i>=0;i--) free(cm->map[i]);
	free(cm->map);
	free(cm);
}


/** Gravity Functions **/

/* Returns jump speed according to gravity */
int applyGravity(int js) {
	return js+GRAVITY;
}
