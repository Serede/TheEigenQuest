#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "player.h"
#include "boss.h"
#include "skill.h"
#include "messages.h"
#include "combat_interface.h"

typedef struct _combat_t combat_t;
typedef struct _combat_skel combat_skel;

#define PLAYER_PIXMAP "stages/common/jay_stand.pixmap"
#define BILIAR_PIXMAP "stages/stage1/biliar.pixmap"
#define CYCLOPS_PIXMAP "stages/stage2/cyclops.pixmap"
#define IRONMAN_PIXMAP "stages/stage3/ironman.pixmap"
#define CUERVS_PIXMAP "stages/stage4/cuervs.pixmap"
#define BOSS_N 4

#define CURE_PROBABILITY 10

#define POISON_DAMAGE 0.90
#define MINDFUCK_DAMAGE 2
#define NOT_BLIND_PRECISION DEFAULT_PLAYER_PRECISION
#define BLIND_PRECISION 0.75
#define PARAL_PROBABILITY 30

#define SKILL_MAX_NAME 32
#define HALF_LIFE(b) (getBossHealth(b))/2
#define QUARTER_LIFE(b) (getBossHealth(b))/4

#define GREEN_THRESHOLD 0.40
#define YELLOW_THRESHOLD 0.20

/*Main functions*/

/*Inits turn based combat, with the player, the boss and
 the interface*/
bool startCombat(stage cmb, short u_skills, short max_health);
/*Load-unload messages that the combat will use*/
char** loadMessages(stage cmb);
void unloadMessages(char** messages);

/*Combat skeleton*/

/*Initialize the data needed for the combat interface*/
combat_skel* initSkeleton(skill* skill_list, player* p, boss* b);
/*Destroys the interface*/
void destroySkeleton(combat_skel* cs);
/*Loads the combat options*/
char** loadOptions(skill* s_list, player* p);
/*Unloads options*/
void unloadOptions(char** options, short howmany);

/*Combat functions*/

/*Combat loop*/
status combatLoop(combat_skel* cs, player* p, boss* b, char** messages, bool* result);
/*Something like the boss AI*/
short setBossChoice(boss* b);

/*Action attempts to use a skill from the attacker and
 updates the fighters, while setting the output text */
status playerAction(combat_skel* cs, player* p, boss* b, short sindex, short* tryagain);
status bossAction(combat_skel* cs, boss* b, player* p, short sindex);
/*Returns the damage inflicted by an action*/
short attack(skill* s, float precision, short sindex, bool* failed);
/*Returns the state inflicted by an action*/
state inflict(skill* s, float precision, short sindex);

#endif
