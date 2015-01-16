#ifndef _ITEM_H
#define _ITEM_H

#include "types.h"

#define ITEMS_N 3
#define ITEM_WATER_AMOUNT 2
#define ITEM_FIRE_AMOUNT 2
#define ITEM_LIGHT_AMOUNT 1

typedef enum {WATER_AMMO=0, FIRE_AMMO, LIGHT_AMMO} item_t;

/* Returns element affected by item */
element getItemElement(item_t item);

/* Returns item affecting element */
item_t getElementItem(element type);

/* Returns amount of ammo to reload by item */
short getItemAmount(item_t item);

#endif
