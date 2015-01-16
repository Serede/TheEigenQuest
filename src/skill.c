#include "skill.h"

struct _skill {
	char name[MAX_SNAME], description[MAX_SDESC];
	short damage, target, max_pp;
	state effect;
	short attack_precision, state_precision;
};

/* returns state string */
char *getStateString(state st) {
	switch(st) {
		case POISON: return STRING_POISON;
		case BLIND: return STRING_BLIND;
		case MINDFUCKED: return STRING_MINDFUCKED;
		case PARAL: return STRING_PARALIZED;
		case ATT_DOWN:
		case DEF_DOWN:
		case ATT_UP:
		case DEF_UP:
			    return STRING_STATS;
		default: return "None";
	}
}

/* loads skills data from a file into an array */
skill *loadSkills(){
	int i;
	short damage=-1, target=-1, max_pp=-1, effect=-1, attack_precision=-1, state_precision=-1;
	char name[MAX_SNAME], description[MAX_SDESC], buffer[2*MAX_SDESC];
	char *start, *end;
	
	skill *list = (skill*)malloc(SKILL_N*sizeof(skill));
	if(list==NULL) return NULL;
	
	FILE *f = fopen(SKILL_FILE,"r");
	if(f==NULL) return NULL;
	
	for(i=0;i<SKILL_N;i++){
		fgets(buffer,2*MAX_SDESC,f);
		start=buffer; end=buffer;
		if(*buffer=='#') continue;
		
		/*get the name*/
		while(*end && *end!=';') end++;
		*end=EOS; strcpy(name, start);
		
		/*get the description*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; strcpy(description, start);
		
		/*get the target*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; target=atoi(start);
		
		/*get the damage*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; damage=atoi(start);
		
		/*get the effect*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; effect=atoi(start);
		
		/*get the attack precision*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; attack_precision=atoi(start);

		/*get the state precision*/
		start=++end;
		while(*end && *end!=';') end++;
		*end=EOS; state_precision=atoi(start);
		
		/*get the max points*/
		start=++end;
		max_pp=atoi(start);
		
		if(name==NULL||description==NULL||damage==-1||attack_precision==-1||state_precision==-1){
			free(list);
			fclose(f);
			return NULL;
		}
		
		(list+i)->target=target;
		(list+i)->damage=damage;
		(list+i)->effect=effect;
		(list+i)->attack_precision=attack_precision;
		(list+i)->state_precision=state_precision;
		(list+i)->max_pp=max_pp;
		strcpy((list+i)->name,name);
		strcpy((list+i)->description,description);
	}
	fclose(f);
	return list;
}

/* returns skill name of the skill in sindex */
char *getSkillName(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return NULL;
	return (s+sindex)->name;
}

/*returns skill description of the skill in sindex*/
char *getSkillDesc(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return NULL;
	return (s+sindex)->description;
}

/*returns skill target of the skill in sindex*/
short getSkillTarget(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return ERR;
	return (s+sindex)->target;
}

/*returns skill damage of the skill in sindex*/
short getSkillDamage(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return ERR;
	return (s+sindex)->damage;
}

/*returns skill (state) of the skill in sindex*/
state getSkillEffect(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return NONE;
	return (s+sindex)->effect;
}

/*returns skill precision of the skill in sindex*/
short getSkillAttackPrecision(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return ERR;
	return (s+sindex)->attack_precision;
}
short getSkillStatePrecision(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return ERR;
	return (s+sindex)->state_precision;
}

/*returns the maximum points the skill has*/
short getSkillMaxPP(skill *s, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return ERR;
	return (s+sindex)->max_pp;
}


/*unloads skill data - frees the memory of the list*/
void unloadSkills(skill *s){
	if(s==NULL) return;
	free(s);
}
