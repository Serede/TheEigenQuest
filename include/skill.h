#ifndef _SKILL_H
#define _SKILL_H

#include "types.h"

#define SKILL_N 39
#define STATES_N 10
#define ACTUAL_STATES_N 4

/*indicators for the arrays*/
#define MIN 0
#define BLI 1
#define POI 2
#define PAR 3

#define MAX_PPOINTS 99
#define MAX_SNAME 20
#define MAX_SDESC 1024

#define TURNS_AFFLICTED 4

#define ENEMY 0
#define SELF 1

#define SKILL_FILE "texts/skills"

#define STRING_POISON "POI"
#define STRING_BLIND "BLI"
#define STRING_MINDFUCKED "MIN"
#define STRING_PARALIZED "PAR"
#define STRING_STATS "Alter stats"

/*NONE: normal state
  BLIND: downs precision
  POISON: drains health each turn
  MINDFUCKED: you may hit stronger or not at all
  PARALIZED: low probability of success upon attack
  CURED: you return to NONE state*/
typedef enum {NONE=0, ATT_DOWN=1, DEF_DOWN=2, ATT_UP=3, DEF_UP=4, BLIND=5, POISON=6, MINDFUCKED=7, PARAL=8, CURED=9}state;
typedef struct _skill skill;
typedef struct _state_list {
	bool afflicted;
	state state;
}state_list;

/* returns state string */
char *getStateString(state st);

/* loads skills data from a file into an array */
skill *loadSkills();

/* returns skill name for skill in sindex */
char *getSkillName(skill *s, short sindex);

/* returns skill description for skill in sindex */
char *getSkillDesc(skill *s, short sindex);

/*returns skill target of the skill in sindex*/
short getSkillTarget(skill *s, short sindex);

/*returns skill damage of the skill in sindex*/
short getSkillDamage(skill *s, short sindex);

/*returns skill effect of the skill in sindex*/
state getSkillEffect(skill *s, short sindex);

/*returns skill precision of the skill in sindex*/
short getSkillAttackPrecision(skill *s, short sindex);
short getSkillStatePrecision(skill *s, short sindex);

/*returns the maximum points the skill has*/
short getSkillMaxPP(skill *s, short sindex);

/* frees memory for the skill list */
void unloadSkills(skill *s);

#endif
