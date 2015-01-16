#include "boss.h"

struct _boss{
	short id;
	char name[MAX_BOSS_NAME], title[MAX_BOSS_TITLE];
	short health, max_health, sk_n;
	float attack, defense, precision;
	short* skills;
	bool states[ACTUAL_STATES_N];
	short counters[ACTUAL_STATES_N];
};

/*basic functions*/

/*reserves memory for the boss and sets fields to values read from file*/
boss* initBoss(short id){
	if(id<0)
		return NULL;
	
	boss* b=(boss*)malloc(sizeof(boss));
	if(b==NULL) return NULL;
	
	char path[32],fname[12];
	sprintf(fname,"%s%hd%s","boss",id,".boss");
	sprintf(path,"%s%s",BOSS_PATH,fname);
	FILE* fp=fopen(path,"r+");
	if(fp==NULL){ 
		free(b); 
		return NULL; 
	}
	
	char name[MAX_BOSS_NAME], title[MAX_BOSS_TITLE];
	short sk_n, max_health;
	short* skills;

	char buf[256];
	fgets(buf,sizeof(buf),fp);
	char *start, *end;
	start = buf; end = buf;

	while(*end && *end!=';') end++;
	*end=EOS; strcpy(name,start);

	start=++end;
	while(*end && *end!=';') end++;
	*end=EOS; strcpy(title,start);
	
	start=++end;
	while(*end && *end!=';') end++;
	*end=EOS; max_health=atoi(start);
	
	start=++end;
	while(*end && *end!=';') end++;
	*end=EOS; sk_n=atoi(start);

	short sindex;
	skills=(short*)malloc(sk_n*sizeof(short));
	for(int i=0;i<sk_n;i++){ 
		start=++end;
		while(*end && *end!=',') end++;
		*end=EOS; sindex=atoi(start);
	 	skills[i]=sindex;
	}
	
	b->id=id;
	strcpy(b->name,name);
	strcpy(b->title,title);
	b->sk_n=sk_n;
	b->max_health=max_health;
	b->health=max_health;
	b->skills=skills;
	
	if(setBossAttack(b,DEFAULT_BOSS_ATTACK)==ERR){
		free(b);
		return NULL;
	}
	if(setBossDefense(b,DEFAULT_BOSS_DEFENSE)==ERR){
		free(b);
		return NULL;
	}
	if(setBossPrecision(b,DEFAULT_BOSS_PRECISION)==ERR){
		free(b);
		return NULL;
	}
	for(int i=0; i<ACTUAL_STATES_N; i++){
		b->states[i] = false;
		b->counters[i] = TURNS_AFFLICTED;
	}

	return b;
}

/*returns the boss' name*/
short getBossId(boss* b){
	if(b==NULL) return ERR;
	return b->id;
}

/*returns the boss' name*/
char* getBossName(boss* b){
	if(b==NULL) return NULL;
	return b->name;
}

/*returns boss' title*/
char* getBossTitle(boss* b){
	if(b==NULL) return NULL;
	return b->title;
}

/*combat*/

/*health functions*/
short getBossHealth(boss *b){
	if(b==NULL) return ERR;
	return b->health;
}
short getBossMaxHealth(boss *b){
	if(b==NULL) return ERR;
	return b->max_health;
}
status setBossHealth(boss *b, short health){
	if(b==NULL) return ERR;
	if(health<0) health = 0;
	if(health>getBossMaxHealth(b)) health = getBossMaxHealth(b);
	b->health = health;
	return OK;
}

/*state functions*/
status setBossState(boss* b, short stindex){
	if(b==NULL||stindex<0||stindex>STATES_N) return ERR;
	
	switch(stindex){
		case NONE:
			break;
		case ATT_UP:
			if(setBossAttack(b,getBossAttack(b) + BD_ATTACK)==ERR) return ERR;
			break;
		case ATT_DOWN:
			if(setBossAttack(b,getBossAttack(b) - BD_ATTACK)==ERR) return ERR;
			break;
		case DEF_UP:
			if(setBossDefense(b,getBossDefense(b) + BD_DEFENSE)==ERR) return ERR;
			break;
		case DEF_DOWN:
			if(setBossDefense(b,getBossDefense(b) - BD_DEFENSE)==ERR) return ERR;
			break;
		case POISON:
			b->counters[POI] = TURNS_AFFLICTED;
			b->states[POI] = true;
			break;
		case BLIND:
			b->counters[BLI] = TURNS_AFFLICTED;
			b->states[BLI] = true;
			break;
		case MINDFUCKED:
			b->counters[MIN] = TURNS_AFFLICTED;
			b->states[MIN] = true;
			break;
		case PARAL:
			b->counters[MIN] = TURNS_AFFLICTED;
			b->states[PAR] = true;
			break;
		case CURED:
			removeAllBossStates(b);
			break;
		default:
			return ERR;
	}
	
	return OK;
}
void removeAllBossStates(boss* b){
	if(b==NULL) return;
	for(int i=0; i<ACTUAL_STATES_N; i++){
		b->counters[i] = TURNS_AFFLICTED;
		b->states[i] = false;
	}
	return;
}
void removeBossState(boss* b, short stindex){
	if(b==NULL||stindex<0||stindex>STATES_N) return;
	switch(stindex){
		case POISON:
			b->counters[POI] = TURNS_AFFLICTED;
			b->states[POI] = false;
			break;
		case BLIND:
			b->counters[BLI] = TURNS_AFFLICTED;
			b->states[BLI] = false;
			break;
		case MINDFUCKED:
			b->counters[MIN] = TURNS_AFFLICTED;
			b->states[MIN] = false;
			break;
		case PARAL:
			b->counters[PAR] = TURNS_AFFLICTED;
			b->states[PAR] = false;
			break;
		default:
			return;
	}
	return;
}

bool isBossPoisoned(boss* b){
	if(b==NULL) return false;
	return b->states[POI];
}
bool isBossBlind(boss* b){
	if(b==NULL) return false;
	return b->states[BLI];
}
bool isBossMindfucked(boss* b){
	if(b==NULL) return false;
	return b->states[MIN];
}
bool isBossParalized(boss* b){
	if(b==NULL) return false;
	return b->states[PAR];
}

/*number of turns the effect still has*/
short turnsLeftBoss(boss* b, short stindex){
	if(b==NULL||stindex<0||stindex>ACTUAL_STATES_N-1) return ERR;
	return b->counters[stindex];
}
status countDownBoss(boss* b, short stindex){
	if(b==NULL||stindex<0||stindex>ACTUAL_STATES_N-1) return ERR;
	b->counters[stindex] = b->counters[stindex] - 1;
	return OK;
}

/*returns the array with the skills the boss has*/
short* getBossSkills(boss* b){
	if(b==NULL)
		return NULL;
	return b->skills;
}
short getBossSkillN(boss* b){
	if(b==NULL) return ERR;
	return b->sk_n;
}

/*Attack-defense-precision functions*/
float getBossAttack(boss* b){
	if(b==NULL) return -1;
	return b->attack;
}
float getBossDefense(boss *b){
	if(b==NULL) return -1;
	return b->defense;
}
status setBossAttack(boss *b, float value){
	if(b==NULL) return ERR;
	if(value<=0) value=BOSS_MIN_VALUE;
	b->attack = value;
	return OK;	
}
status setBossDefense(boss *b, float value){
	if(b==NULL) return ERR;
	if(value<=0) value=BOSS_MIN_VALUE;
	b->defense = value;
	return OK;
}
float getBossPrecision(boss *b){
	if(b==NULL) return ERR;
	return b->precision;
}
status setBossPrecision(boss *b, float value){
	if(b==NULL) return ERR;
	if(value<=0) value=BOSS_MIN_VALUE;
	b->precision = value;
	return OK;
}


/*frees boss memory*/
void destroyBoss(boss *b){
	if(b==NULL)
		return;
	free(b->skills);
	free(b);
}
