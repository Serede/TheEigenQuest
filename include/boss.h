#ifndef _BOSS_H
#define _BOSS_H


#include "types.h"
#include "skill.h"

#define BOSS_PATH "texts/bosses/"
#define MAX_BOSS_NAME 30
#define MAX_BOSS_TITLE 30
#define DEFAULT_BOSS_ATTACK 1.0
#define DEFAULT_BOSS_DEFENSE 1.0
#define DEFAULT_BOSS_PRECISION 1.0

#define BD_ATTACK 0.10
#define BD_DEFENSE 0.10
#define BOSS_MIN_VALUE 0.01
typedef struct _boss boss;

/*basic functions*/

/*reserves memory for the boss and sets fields to values read from file*/
boss* initBoss(short id);

/*returns the boss' id*/
short getBossId(boss* b);

/*returns the boss' name*/
char* getBossName(boss* b);

/*returns boss' title*/
char* getBossTitle(boss* b);

/*combat*/

/*health functions*/
short getBossHealth(boss *b);
short getBossMaxHealth(boss *b);
status setBossHealth(boss *b, short health);

/*state functions*/
status setBossState(boss* b, short stindex);
void removeAllBossStates(boss* b);
void removeBossState(boss* b, short howmany);

bool isBossPoisoned(boss* b);
bool isBossBlind(boss* b);
bool isBossMindfucked(boss* b);
bool isBossParalized(boss* b);

/*number of turns the effect still has*/
short turnsLeftBoss(boss* b, short stindex);
status countDownBoss(boss* b, short stindex);

/*skills functions*/
short* getBossSkills(boss* b);
short getBossSkillN(boss* b);

/*Attack-defense-precision functions*/
float getBossAttack(boss* b);
float getBossDefense(boss *b);
status setBossAttack(boss *b, float value);
status setBossDefense(boss *b, float value);
float getBossPrecision(boss *b);
status setBossPrecision(boss *b, float value);

/*frees boss memory*/
void destroyBoss(boss *b);

#endif
