#ifndef _PLAYER_H
#define _PLAYER_H


#include "types.h"
#include "skill.h"

#define	PLAYER_NAME "Jay Phi"
#define DEFAULT_PLAYER_ATTACK 1.0
#define DEFAULT_PLAYER_DEFENSE 1.0
#define DEFAULT_PLAYER_PRECISION 1.0
#define DEFAULT_SK_N 4
#define MAX_PLAYER_HEALTH 9999


#define PD_ATTACK 0.25
#define PD_DEFENSE 0.25
#define PLAYER_MIN_VALUE 0.01
typedef struct _player player;

/*basic functions*/

/*reserves memory for the player and sets player fields to secure values*/
player* initPlayer(skill* list, short u_skills, short max_health);

/*combat*/

/*health functions*/
short getPlayerHealth(player *p);
short getPlayerMaxHealth(player* p);
status setPlayerHealth(player *p, short health);

/*state functions*/
status setPlayerState(player* p, short stindex);
void removeAllPlayerStates(player* p);
void removePlayerState(player* p, short stindex);

bool isPlayerPoisoned(player* p);
bool isPlayerBlind(player* p);
bool isPlayerMindfucked(player* p);
bool isPlayerParalized(player* p);

/*number of turns the effect still has*/
short turnsLeftPlayer(player* p, short stindex);
status countDownPlayer(player* p, short stindex);

/*ppoints functions*/
short getPlayerPPoints(player *p, short sindex);
status setPlayerPPoints(player *p, short sindex, short points);

/*Attack-defense-precision functions*/
float getPlayerAttack(player *p);
float getPlayerDefense(player *p);
status setPlayerAttack(player *p, float value);
status setPlayerDefense(player *p, float value);
float getPlayerPrecision(player *p);
status setPlayerPrecision(player *p, float value);

short getPlayerSkillN(player* p);
/*frees player memory*/
void destroyPlayer(player *p);

#endif
