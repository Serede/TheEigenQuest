#include "item.h"

/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Returns element affected by item */
element getItemElement(item_t item) {
	switch(item) {
		case WATER_AMMO: return WATER;
		case FIRE_AMMO: return FIRE;
		case LIGHT_AMMO: return LIGHT;
	}
	return -1;
}

/* Returns item affecting element */
item_t getElementItem(element type) {
	switch(type) {
		case WATER: return WATER_AMMO;
		case FIRE: return FIRE_AMMO;
		case LIGHT: return LIGHT_AMMO;
	}
	return -1;
}

/* Returns amount of ammo to reload by item */
short getItemAmount(item_t item) {
	switch(item) {
		case WATER_AMMO: return ITEM_WATER_AMOUNT;
		case FIRE_AMMO: return ITEM_FIRE_AMOUNT;
		case LIGHT_AMMO: return ITEM_LIGHT_AMOUNT;
	}
}
