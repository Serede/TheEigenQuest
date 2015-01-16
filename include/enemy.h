#ifndef _ENEMY_H
#define _ENEMY_H

#include "types.h"

#define ENEMIES_N 19

typedef enum {
	ZITHERS=0,
	FAIRY=1,
	ZOMBIE=2,
	FLAME=3,
	GHOST=4,
	DRAGON1=5,
	DRAGON2=6,
	DRAGON3=7,
	DRAGON4=8,
	DRAGON5=9,
	DRAGON6=10,
	DRAGON7=11,
	DRAGON8=12,
	DRAGON9=13,
	CHARLOTTE=14,
	BAT=15,
	CROW1=16,
	CROW2=17,
	CERBERUS=18
} enemy_t;

/* Tells whether a enemy is killed by a bullet, 2 means bullet remains */
short isEnemyKilledBy(enemy_t enemy, element bullet);

#endif
