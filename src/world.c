#include "world.h"

/* PUBLIC DEFINITIONS */

struct _World {
	level lvl; /* Current level */
	joukyou status; /* World status */

	bool jumping; /* Whether player is jumping */
	int yspeed, xspeed; /* Player speeds */

	short weapon; /* Active weapon */
	Weapon *weapons; /* Array of weapons */

	CollisionMap *cm; /* Map of collisions */
	bool colliding[4]; /* 4-sides collision */
	bool need_collide; /* Whether it needs collision to be recomputed */

	Layout *lo; /* Layout subsystem */
	ColorPair *cp; /* Colorpairs for output */

	bool gm; /* Garrido mode */
};


/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

/* Tells which weapon is unlocked in each level */
short levelWeapon(level lvl) {
	switch(lvl) {
		case TUTO: return BASIC;
		case LVL1: return WATER;
		case LVL2: return FIRE;
		case LVL3: return LIGHT;
		default: return BASIC;
	}
}


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Creates world */
World *initWorld(level lvl, short unlocked_weapons) {
	World *w;
	char buf[FILE_MAX];
	short i;

	if(unlocked_weapons<0||unlocked_weapons>ELEMENTS_N) return NULL;

	/* Allocate world */
	w = (World*)malloc(sizeof(World));
	if(w==NULL) return NULL;
	
	/* Allocate array of weapons */
	w->weapons = initWeapons();
	if(w->weapons==NULL) {
		free(w);
		return NULL;
	}

	/* Garrido Mode */
	if(!unlocked_weapons) {
		w->gm = true;
		unlocked_weapons = ELEMENTS_N;
	}
	/* Normal mode */
	else w->gm = false;

	/* Unlock weapons */
	for(i=0;i<unlocked_weapons;i++) {
		if(unlockWeapon(w->weapons,i)==ERR) {
			destroyWeapons(w->weapons);
			free(w);
			return NULL;
		}
	}

	/* Allocate map of collisions */
	switch(lvl) {
		case TUTO:
			strcpy(buf,TUTO_FILE);
			break;
		case LVL1:
			strcpy(buf,LVL1_FILE);
			break;
		case LVL2:
			strcpy(buf,LVL2_FILE);
			break;
		case LVL3:
			strcpy(buf,LVL3_FILE);
			break;
		case LVL4:
			strcpy(buf,LVL4_FILE);
			break;
		default:
			destroyWeapons(w->weapons);
			free(w);
			return NULL;
	}
	strcat(buf,".cm");
	w->cm = loadCollisionMap(buf);
	if(w->cm==NULL) {
		destroyWeapons(w->weapons);
		free(w);
		return NULL;
	}

	/* Allocate colorpairs */
	w->cp = initColorPairs();
	if(w->cp==NULL) {
		destroyCollisionMap(w->cm);
		destroyWeapons(w->weapons);
		free(w);
		return NULL;
	}

	/* Allocate layout */
	w->lo = initLayout(lvl,w->cp);
	if(w->lo==NULL) {
		destroyColorPairs(w->cp);
		destroyCollisionMap(w->cm);
		destroyWeapons(w->weapons);
		free(w);
		return NULL;
	}

	/* Set current level */
	w->lvl = lvl;

	/* Set jumping to false */
	w->jumping = false;

	/* Set speeds to 0 */
	w->yspeed = 0;
	w->xspeed = 0;

	/* Set active weapon */
	w->weapon = WORLD_DEFAULT_WEAPON;

	/* Set collisions to false */
	for(i=0;i<4;i++) w->colliding[i] = false;

	/* Ask for collision recompute */
	w->need_collide = true;

	/* It is alive! Launch intro */
	w->status = KAISHI;

	return w;
}

/* Returns world status */
joukyou worldGetStatus(World *w) {
	if(w==NULL) return YAME;
	return w->status;
}
/* Pauses world */
status pauseWorld(World *w) {
	if(w==NULL) return ERR;
	w->status = TEISHI;
	return OK;
}
/* Stops world */
status stopWorld(World *w) {
	if(w==NULL) return ERR;
	w->status = YAME;
	return OK;
}

/* Moves player left or right */
status worldPlayerWalk(World *w, side side) {
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	switch(side) {
		case LEFT:
			if(!w->jumping) {
				if(!w->need_collide && !w->colliding[LEFT]) {
					w->need_collide = true;
					return loMovePlayer(w->lo,0,-1);
				}
			}
			else w->xspeed = -WORLD_JUMP_XSPEED;
			break;
		case RIGHT:
			if(!w->jumping) {
				if(!w->need_collide && !w->colliding[RIGHT]) {
					w->need_collide = true;
					return loMovePlayer(w->lo,0,1);
				}
			}
			else w->xspeed = WORLD_JUMP_XSPEED;
			break;
		default:
			return ERR;
	}
	return OK;
}
/* Starts player jump */
status worldPlayerJump(World *w) {
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	if(!w->need_collide && !w->colliding[UP] && !w->jumping) {
		w->jumping = true;
		w->yspeed = w->gm? WORLD_JUMP_YSPEED_GM:WORLD_JUMP_YSPEED;
	}
	return OK;
}
/* Switches player active weapon */
status worldWeaponSwitch(World *w) {
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	do {
		w->weapon++;
		w->weapon %= ELEMENTS_N;
	} while (!isWeaponUnlocked(w->weapons,w->weapon));
	return OK;
}
/* Shoots a bullet with active weapon */
status worldWeaponShoot(World *w) {
	int y, x;
	orientation ori;
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	if(!getWeaponAmmo(w->weapons,w->weapon)) return OK;
	if(loGetPlayerInfo(w->lo,&y,&x,&ori)==ERR) return ERR;
	x += ori==FORWARD? WORLD_PLAYER_WIDTH:-1;
	y += WORLD_PLAYER_HEIGHT/3;
	if(loCreateBullet(w->lo,y,x,w->weapon)==ERR) return ERR;
	if(loSetPlayerStance(w->lo,w->jumping?JUMPSHOOT:SHOOT)==ERR) return ERR;
	return changeWeaponAmmo(w->weapons,w->weapon,-1);
}

/* Applies gravity */
status worldUpdateGravity(World *w) {
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	if(!w->need_collide && !w->colliding[DOWN])
		w->yspeed = applyGravity(w->yspeed);
	return OK;
}
/* Applies collision to speeds */
status worldUpdateSpeed(World *w) {
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	if(w->yspeed>0 && !w->need_collide && w->colliding[UP])
		w->yspeed = 0;
	if(w->yspeed<=0 && !w->need_collide && w->colliding[DOWN]) {
		w->yspeed = 0;
		w->xspeed = 0;
		w->jumping = false;
	}
	if((w->xspeed<0 && !w->need_collide && w->colliding[LEFT]) ||\
		(w->xspeed>0 && !w->need_collide && w->colliding[RIGHT])) {
		w->xspeed = 0;
		//w->jumping = false;
	}
	return OK;
}
/* Updates player stance */
status worldUpdateStance(World *w) {
	if(w==NULL) return ERR;
	if(w->status==YAME||w->status==SHINDA) return OK;
	if(!w->yspeed && !w->xspeed) {
		if(loSetPlayerStance(w->lo,STAND)==ERR) return ERR;
	}
	else {
		if(loSetPlayerStance(w->lo,JUMP)==ERR) return ERR;
	}
	return OK;
}
/* Realculates 4-side collision booleans */
status worldUpdateCollision(World *w) {
	int y, x, p_y, p_x;
	bool colliding;
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	if(w->need_collide) {
		if(loGetPlayerInfo(w->lo,&p_y,&p_x,NULL)==ERR) return ERR;

		/* Up */
		for(colliding=false,x=p_x;x<p_x+WORLD_PLAYER_WIDTH;x++)
			colliding = colliding || isColliding(w->cm,p_y-1,x);
		w->colliding[UP] = colliding;

		/* Down */
		for(colliding=false,x=p_x;x<p_x+WORLD_PLAYER_WIDTH;x++)
			colliding = colliding || isColliding(w->cm,p_y+WORLD_PLAYER_HEIGHT,x);
		w->colliding[DOWN] = colliding;

		/* Right */
		for(colliding=false,y=p_y;y<p_y+WORLD_PLAYER_HEIGHT;y++)
			colliding = colliding || isColliding(w->cm,y,p_x+WORLD_PLAYER_WIDTH);
		w->colliding[RIGHT] = colliding;

		/* Left */
		for(colliding=false,y=p_y;y<p_y+WORLD_PLAYER_HEIGHT;y++)
			colliding = colliding || isColliding(w->cm,y,p_x-1);
		w->colliding[LEFT] = colliding;

		/* Collision recomputed */
		w->need_collide = false;
	}

	return OK;
}

/* Moves and clears bullets */
status worldUpdateBullets(World *w) {
	int i;
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;
	for(i=0;i<ELEMENTS_N;i++) {
		if(loMoveBullets(w->lo,0,getWeaponBulletSpeed(w->weapons,i),i)==ERR)
			return ERR;
	}
	if(loCleanBullets(w->lo)==ERR) return ERR;
	return OK;
}

/* Moves player applying current momentum */
status worldApplyMomentum(World *w) {
	int i;
	if(w==NULL) return ERR;

	if(w->yspeed>0) { /* Up */
		for(i=0;i<abs(w->yspeed);i++) {
			if(!w->need_collide && w->colliding[UP]==true) break;
			if(loMovePlayer(w->lo,-1,0)==ERR) return ERR;
			w->need_collide = true;
			worldUpdateCollision(w);
		}
	}

	else if(w->yspeed<0) { /* Down */
		for(i=0;i<abs(w->yspeed);i++) {
			if(!w->need_collide && w->colliding[DOWN]==true) break;
			if(loMovePlayer(w->lo,1,0)==ERR) return ERR;
			w->need_collide = true;
			worldUpdateCollision(w);
		}
	}

	if(w->xspeed>0) { /* Right */
		for(i=0;i<abs(w->xspeed);i++) {
			if(!w->need_collide && w->colliding[RIGHT]==true) break;
			if(loMovePlayer(w->lo,0,1)==ERR) return ERR;
			w->need_collide = true;
			worldUpdateCollision(w);
		}
	}

	else if(w->xspeed<0) { /* Left */
		for(i=0;i<abs(w->xspeed);i++) {
			if(!w->need_collide && w->colliding[LEFT]==true) break;
			if(loMovePlayer(w->lo,0,-1)==ERR) return ERR;
			w->need_collide = true;
			worldUpdateCollision(w);
		}
	}
	return OK;
}

/* Applies touch effects */
status worldApplyTouch(World *w) {
	int p_y, p_x, lvl_h, lvl_w, item_y, item_x;
	item_t item=-1;
	
	if(w==NULL) return ERR;
	if(w->status==YAME) return OK;

	if(loGetPlayerInfo(w->lo,&p_y,&p_x,NULL)==ERR) return ERR;
	if(loGetBlocksSize(w->lo,&lvl_h,&lvl_w)==ERR) return ERR;

	/* Check if player has fallen */
	if(p_y>lvl_h) {
		if(loSetPlayerStance(w->lo,DEAD)==ERR) return ERR;
		w->status = SHINDA;
		return OK;
	}

	/* Check if player has stepped onto an enemy */
	switch(loMatchPlayer(w->lo,WORLD_PLAYER_HEIGHT,WORLD_PLAYER_WIDTH,\
		&item,&item_y,&item_x)) {
		case 1:
			if(item==-1) return ERR;
			if(changeWeaponAmmo(w->weapons,getItemElement(item),\
				getItemAmount(item))==ERR) return ERR;
			if(loDestroyItem(w->lo,item_y,item_x)==ERR) return ERR;
			break;
		case 2:
			if(loSetPlayerStance(w->lo,DEAD)==ERR) return ERR;
			w->status = SHINDA;
			return OK;
		case 0:
			break;
		default:
			return ERR;
	}

	/* Check if player has beaten level */
	if(p_x+WORLD_PLAYER_WIDTH>=lvl_w-WORLD_SUCCESS_THRESHOLD) w->status = TASSEI;

	return OK;
}

/* Kills enemies touching bullets */
status worldApplyKill(World *w) {
	unsigned int matches;
	element *b_t=NULL;
	enemy_t *e_t=NULL;
	int i, *b_y=NULL, *b_x=NULL, *e_y=NULL, *e_x=NULL;
	item_t spawn;

	/* Kill enemies with bullets */
	matches = loMatchBulletEnemy(w->lo,&b_t,&b_y,&b_x,&e_t,&e_y,&e_x);
	if(matches==-1) return ERR;
	for(i=0;i<matches;i++) {
		switch(isEnemyKilledBy(e_t[i],b_t[i])) {
			case 0:
				if(loDestroyBullet(w->lo,b_y[i],b_x[i])==ERR) {
					if(b_t!=NULL) free(b_t);
					if(b_y!=NULL) free(b_y);
					if(b_x!=NULL) free(b_x);
					if(e_t!=NULL) free(e_t);
					if(e_y!=NULL) free(e_y);
					if(e_x!=NULL) free(e_x);
					return ERR;
				}
				break;
			case 1:
				if(loDestroyBullet(w->lo,b_y[i],b_x[i])==ERR) {
					if(b_t!=NULL) free(b_t);
					if(b_y!=NULL) free(b_y);
					if(b_x!=NULL) free(b_x);
					if(e_t!=NULL) free(e_t);
					if(e_y!=NULL) free(e_y);
					if(e_x!=NULL) free(e_x);
					return ERR;
				}
			case 2:
				if(loDestroyEnemy(w->lo,e_y[i],e_x[i])==ERR) {
					if(b_t!=NULL) free(b_t);
					if(b_y!=NULL) free(b_y);
					if(b_x!=NULL) free(b_x);
					if(e_t!=NULL) free(e_t);
					if(e_y!=NULL) free(e_y);
					if(e_x!=NULL) free(e_x);
					return ERR;
				}
				spawn = getElementItem(getWeaponNeedAmmo(w->weapons));
				if(spawn>=0&&spawn<ITEMS_N) {
					if(loCreateItem(w->lo,e_y[i],e_x[i],\
								spawn)==ERR) {
						if(b_t!=NULL) free(b_t);
						if(b_y!=NULL) free(b_y);
						if(b_x!=NULL) free(b_x);
						if(e_t!=NULL) free(e_t);
						if(e_y!=NULL) free(e_y);
						if(e_x!=NULL) free(e_x);
						return ERR;
					}
				}
				break;
		}
	}

	if(b_t!=NULL) free(b_t);
	if(b_y!=NULL) free(b_y);
	if(b_x!=NULL) free(b_x);
	if(e_t!=NULL) free(e_t);
	if(e_y!=NULL) free(e_y);
	if(e_x!=NULL) free(e_x);
	return OK;
}

/* Prints world */
status printWorld(WINDOW *win, World *w) {
	int s_h, s_w;
	char str[WORLD_AMMO_LEN];
	short exit;
	char *name, *desc1, *desc2;
	if(win==NULL||w==NULL) return ERR;
	if(printLayout(win,w->lo,w->cp)==ERR) return ERR;
	if(isWeaponUnlocked(w->weapons,WATER)) {
		sprintf(str,"%02hd",getWeaponAmmo(w->weapons,WATER));
		showText(win,WORLD_AMMO_Y,WORLD_AMMO_X,\
					str,WORLD_AMMO_CP1,w->weapon==WATER);
	}
	if(isWeaponUnlocked(w->weapons,FIRE)) {
		sprintf(str,"%02hd",getWeaponAmmo(w->weapons,FIRE));
		showText(win,WORLD_AMMO_Y,WORLD_AMMO_X+WORLD_AMMO__,\
					str,WORLD_AMMO_CP2,w->weapon==FIRE);
	}
	if(isWeaponUnlocked(w->weapons,LIGHT)) {
		sprintf(str,"%01hd",getWeaponAmmo(w->weapons,LIGHT));
		showText(win,WORLD_AMMO_Y,WORLD_AMMO_X+2*WORLD_AMMO__,\
					str,WORLD_AMMO_CP3,w->weapon==LIGHT);
	}
	switch(w->status) {
		case TEISHI: /* Pause */
			if(loGetScreenSize(w->lo,&s_h,&s_w)==ERR) return ERR;
			exit = showDialog(win,s_h,s_w,WORLD_PAUSE_TITLE,WORLD_PAUSE_BODY,1,0);
			if(exit==-1) return ERR;
			w->status = exit? YAME:GENKI;
			break;
		case KAISHI: /* Intro */
			if(loGetScreenSize(w->lo,&s_h,&s_w)==ERR) return ERR;
			switch(w->lvl) {
				case TUTO:
					exit = showDialog(win,s_h,s_w,WORLD_INTRO_T_TITLE,WORLD_INTRO_T_BODY,0,0);
					break;
				case LVL1:
					exit = showDialog(win,s_h,s_w,WORLD_INTRO_1_TITLE,WORLD_INTRO_1_BODY,0,0);
					break;
				case LVL2:
					exit = showDialog(win,s_h,s_w,WORLD_INTRO_2_TITLE,WORLD_INTRO_2_BODY,0,0);
					break;
				case LVL3:
					exit = showDialog(win,s_h,s_w,WORLD_INTRO_3_TITLE,WORLD_INTRO_3_BODY,0,0);
					break;
				case LVL4:
					exit = showDialog(win,s_h,s_w,WORLD_INTRO_4_TITLE,WORLD_INTRO_4_BODY,0,0);
					break;
				default:
					exit = -1;
			}
			if(exit==-1) return ERR;
			w->status = GENKI;
			break;
		case TASSEI: /* Success */
			if(loGetScreenSize(w->lo,&s_h,&s_w)==ERR) return ERR;
			if(w->lvl==TUTO) {
				exit = showDialog(win,s_h,s_w,WORLD_SUCCESS_TITLE,WORLD_SUCCESS_BODY,0,0);
				if(exit==-1) return ERR;
			}
			exit = showDialog(win,s_h,s_w,\
					w->lvl<4?getWeaponName(w->weapons,levelWeapon(w->lvl)):WORLD_SUCCESS_END_TITLE,\
					w->lvl<4?getWeaponDesc(w->weapons,levelWeapon(w->lvl)):WORLD_SUCCESS_END_BODY,\
					0,0
				);
			if(exit==-1) return ERR;
			break;
		case SHINDA: /* Dead */
			if(loGetScreenSize(w->lo,&s_h,&s_w)==ERR) return ERR;
			exit = showDialog(win,s_h,s_w,WORLD_DEAD_TITLE,WORLD_DEAD_BODY,1,0);
			if(exit==-1) return ERR;
			if(exit) w->status = YAME;
			break;
	}
	refresh();
	return OK;
}

/* Frees world */
void destroyWorld(World *w) {
	if(w==NULL) return;
	destroyLayout(w->lo);
	destroyColorPairs(w->cp);
	destroyCollisionMap(w->cm);
	destroyWeapons(w->weapons);
	free(w);
}
