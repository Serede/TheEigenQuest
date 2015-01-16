#ifndef _WEAPON_H
#define _WEAPON_H

#include "types.h"

#define WEAPONS_FILE "texts/weapons"
#define W_MAX_NAME_LEN 20
#define W_MAX_DESC_LEN 200
#define W_DICE_SIDES ELEMENTS_N
#define W_DICE_LOADS 30,30,30,10
#define W_DICE_RES 10

typedef struct _Weapon Weapon;

/* Loads array of weapons */
Weapon *initWeapons();

/* Returns weapon current ammo */
short getWeaponAmmo(Weapon *w, short windex);

/* Returns windex of weapon that needs ammo */
short getWeaponNeedAmmo(Weapon *w); 

/* Increases or decreases weapon ammo by given amount */
status changeWeaponAmmo(Weapon *w, short windex, short amount);

/* Returns weapon bullet speed */
short getWeaponBulletSpeed(Weapon *w, short windex);

/* Checks whether a weapon is unlocked */
bool isWeaponUnlocked(Weapon *w, short windex);

/* Sets weapon as unlocked */
status unlockWeapon(Weapon *w, short windex);

/* Returns pointer to weapon name */
char *getWeaponName(Weapon *w, short windex);

/* Returns pointer to weapon description */
char *getWeaponDesc(Weapon *w, short windex);

/* Frees array of weapons */
void destroyWeapons(Weapon *w);

#endif
