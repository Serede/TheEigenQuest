#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "types.h"

#define GRAVITY -1

typedef struct _CollisionMap CollisionMap;

/** Collision Functions **/

/* Load CollisionMap from file */
CollisionMap *loadCollisionMap(char *file);

/* Returns whether input coordinates collide */
bool isColliding(CollisionMap *cm, int y, int x);

/* Frees a given CollisionMap */
void destroyCollisionMap(CollisionMap *cm);


/** Gravity Functions **/

/* Returns result of applying gravity to given speed */
int applyGravity(int speed);

#endif
