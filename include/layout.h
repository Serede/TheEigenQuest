#ifndef _LAYOUT_H
#define _LAYOUT_H

#include "color.h"
#include "item.h"
#include "enemy.h"
#include "types.h"

#define COMMON_FILE "levels/cmmn"
#define TUTO_FILE "levels/tuto"
#define LVL1_FILE "levels/lvl1"
#define LVL2_FILE "levels/lvl2"
#define LVL3_FILE "levels/lvl3"
#define LVL4_FILE "levels/lvl4"

#define LO_DEFAULT_ORI FORWARD
#define LO_BG_MOD 2
#define LO_ANY -1

typedef enum {FORWARD=0, BACKWARD=1} orientation;

typedef enum {STAND=0, SHOOT, JUMP, JUMPSHOOT, DEAD} stance;

typedef struct _Layout Layout;

/* Loads every pixmap into memory */
Layout *initLayout(level lvl, ColorPair *cp);

/* Blocks functions */
status loGetBlocksSize(Layout *lo, int *h, int *w);

/* Fixed_bg pixmap functions */
status loGetScreenSize(Layout *lo, int *h, int *w);

/* Player pixmap functions */
status loGetPlayerInfo(Layout *lo, int *y, int *x, orientation *ori);
status loMovePlayer(Layout *lo, int ystep, int xstep);
status loSetPlayerStance(Layout *lo, stance st);
status loSetPlayerOri(Layout *lo, orientation ori);

/* Enemies pixmaps functions */
status loMoveEnemy(Layout *lo, int y, int x, int ystep, int xstep);
status loDestroyEnemy(Layout *lo, int y, int x);

/* Items pixmaps functions */
status loCreateItem(Layout *lo, int y, int x, item_t type);
status loMoveItem(Layout *lo, int y, int x, int ystep, int xstep);
status loDestroyItem(Layout *lo, int y, int x);

/* Bullets pixmaps functions */
status loCreateBullet(Layout *lo, int y, int x, element type);
status loMoveBullets(Layout *lo, int ystep, int xstep, element type);
status loDestroyBullet(Layout *lo, int y, int x);
status loCleanBullets(Layout *lo);

/* Returns number of bullets matching enemies and stores their types and positions */
unsigned int loMatchBulletEnemy(Layout *lo, element **b_t, int **b_y, int **b_x, enemy_t **e_t, int **e_y, int **e_x);

/* Returns what is at player location:
 * -1=error, 0=nothing, 1=item, 2=enemy
 * Also stores item info */
short loMatchPlayer(Layout *lo, int p_h, int p_w, item_t *item, int *y, int *x);

/* Prints layout in window */
status printLayout(WINDOW *win, Layout *lo, ColorPair *cp);

/* Frees memory of given layout */
void destroyLayout(Layout *lo);

#endif
