#ifndef _WORLD_H
#define _WORLD_H

#include "layout.h"
#include "weapon.h"
#include "item.h"
#include "enemy.h"
#include "physics.h"
#include "utils.h"
#include "types.h"

/* Ammo display macros */
#define WORLD_AMMO_Y 1
#define WORLD_AMMO_X 2
#define WORLD_AMMO__ 4
#define WORLD_AMMO_LEN 3
#define WORLD_AMMO_CP1 49
#define WORLD_AMMO_CP2 25
#define WORLD_AMMO_CP3 53

/* Pause macros */
#define WORLD_PAUSE_TITLE "PAUSE"
#define WORLD_PAUSE_BODY "Paused. What do you want to do?"

/* Intro macros */
#define WORLD_INTRO_T_TITLE "TUTORIAL"
#define WORLD_INTRO_T_BODY	"Those weird signs somehow/"\
				"printed on the grass show/"\
				"the controls. It will not/"\
				"be necessary to calculate/"\
				"any single characteristic/"\
				"polinomial, do not worry."
#define WORLD_INTRO_1_TITLE "LEVEL 1: EPS"
#define WORLD_INTRO_1_BODY	"You were peacefully eating a hamburguer with salchipapas at the cafeteria/"\
       				"when a few chicks entered from the corridor screaming out loud. Something/"\
				"about faries and zombies is what you can understand from people babbling./"\
				"Zombies... cool! But wait, why on Earth would there be faries in the EPS?/"\
				"...NO WAY! Cou-could it be... THAT LEGEND!? Better take a look out there."
#define WORLD_INTRO_2_TITLE "LEVEL 2: Nitrogen Pass"
#define WORLD_INTRO_2_BODY	"Before leaving, Dr.  Biliar gave you some useful information: There  seems/"\
				"to be an evil spirit sowing chaos and havoc around the 17th Module in  the/"\
				"Faculty of Science.  A twisted mind with no mercy  is  trying  to  dualize/"\
				"everything within the face of the  planet  into  simple  eigenvalues,  and/"\
				"taking over the world is claimed to  be  his  final  goal.    According to/"\
				"rumours, the culprit is Cuervs, the Evil Algebra Crow.  You thence rush to/"\
				"the shortcut leading to the Faculty of Science."
#define WORLD_INTRO_3_TITLE "LEVEL 3: Faculty of Science"
#define WORLD_INTRO_3_BODY	"Eventually, Cyclops lets you pass CIREL under the gaze of  jealous  students/"\
				"and you are granted access to an utterly chaotic  Faculty  of  Sience, where/"\
				"charming physicians and old-fashioned dudes boasting of their ID  cards  try/"\
				"to recruit you for their lewd club activities. You'd better watch out, don't/"\
				"get close to them if you don't want to be fooled!  Some biologists have also/"\
				"released dozens of bats infected by a lethal disease and a group of students/"\
				"from Biochemistry were talking about a weird puzzle near the 17th  module.../"\
				"What might that be?   Are you perhaps to be proved as a true  mathematician?/"\
				"The time is running out, show them the power of Maths, Jay Phi!"
#define WORLD_INTRO_4_TITLE "LEVEL 4: Secret Math Library"
#define WORLD_INTRO_4_BODY	"Ironman has granted you access to the 17th module, the one where the/"\
				"Mathematics classes used to be before The Algebra Crow took control./"\
				"The door of the Library, closed before, to contain the secret powers/"\
				"of knowledge at bay, is now destroyed. The entrance, wide open. What/"\
				"dark math has Cuervs unleashed from this foul pit of confusion? This/"\
				"is the moment, Jay Phi. Prove that your path is right:  venture into/"\
				"darkness and stop the dual space expansion before it's too late!"

/* Success macros */
#define WORLD_SUCCESS_TITLE "TUTORIAL COMPLETED"
#define WORLD_SUCCESS_BODY	"You now master the basics and are ready/"\
       				"to begin this unforgettable eigenquest./"\
				"A jeopardous dualized world lies beyond/"\
				"awaiting the arrival of a young hero.  /"\
				"                                       /"\
				"It is dangerous to be alone! Take this."
#define WORLD_SUCCESS_END_TITLE "CUERVS OFFICE KEY"
#define WORLD_SUCCESS_END_BODY "You have overcome the dangers  /"\
			       "of the Library! This looks like/"\
			       "the key to the big door. Cuervs/"\
			       "waits just ahead..."

/* Dead macros */
#define WORLD_DEAD_TITLE "YOU ARE DEAD"
#define WORLD_DEAD_BODY	"You can hear a crow cawing into the distance. It/"\
			"resembles the most evil laughter in history. You/"\
			"were diagonalized into a matrix of complex eigen/"\
			"values by some dark algebra of Wicked Cuervs and/"\
			"your lifeless body lies corrupted on the ground."


/* World itself macros */
#define WORLD_DEFAULT_WEAPON 0
#define WORLD_PLAYER_HEIGHT 8
#define WORLD_PLAYER_WIDTH 15 
#define WORLD_JUMP_XSPEED 5
#define WORLD_JUMP_YSPEED 5
#define WORLD_JUMP_YSPEED_GM 2048
#define WORLD_SUCCESS_THRESHOLD 10

typedef struct _World World;

/* -2:Stop, -1:Pause, 0:Intro, 1:Alright, 2:Success, 3:Dead */
typedef enum {YAME=-2, TEISHI=-1, KAISHI=0, GENKI=1, TASSEI=2, SHINDA=3} joukyou;

/* Creates world */
World *initWorld(level lvl, short unlocked_weapons);

/* Returns world status */
joukyou worldGetStatus(World *w);
/* Pauses world */
status pauseWorld(World *w);
/* Stops world */
status stopWorld(World *w);

/* Moves player left or right */
status worldPlayerWalk(World *w, side side);
/* Starts player jump */
status worldPlayerJump(World *w);
/* Switches player active weapon */
status worldWeaponSwitch(World *w);
/* Shoots a bullet with active weapon */
status worldWeaponShoot(World *w);

/* Applies gravity */
status worldUpdateGravity(World *w);
/* Applies collision to speeds */
status worldUpdateSpeed(World *w);
/* Updates player stance */
status worldUpdateStance(World *w);
/* Realculates 4-side collision booleans */
status worldUpdateCollision(World *w);
/* Moves and clears bullets */
status worldUpdateBullets(World *w);
/* Moves player applying current momentum */
status worldApplyMomentum(World *w);
/* Applies touch effects */
status worldApplyTouch(World *w);
/* Kills enemies touching bullets */
status worldApplyKill(World *w);

/* Prints world */
status printWorld(WINDOW *win, World *w);

/* Frees world */
void destroyWorld(World *w);

#endif
