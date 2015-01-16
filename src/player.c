#include "player.h"

struct _player {
	short health, max_health, sk_n;
	short* ppoints;
	float attack, defense, precision;
	bool states[ACTUAL_STATES_N];
	short counters[ACTUAL_STATES_N];
};

/*reserves memory for the player and sets player fields to secure values*/
player* initPlayer(skill* list, short u_skills, short max_health){
	if(list==NULL||u_skills<0||max_health<=0) return NULL;
	player *p;
	
	p = (player*)malloc(sizeof(player));
	if(p==NULL) return NULL;

	p->max_health = max_health;
        if(setPlayerHealth(p,max_health)==ERR){
		free(p);
                return NULL;
	}
	
	if(setPlayerAttack(p,DEFAULT_PLAYER_ATTACK)==ERR){
		free(p);
		return NULL;
	}
	if(setPlayerDefense(p,DEFAULT_PLAYER_DEFENSE)==ERR){
		free(p);
		return NULL;
	}
	
	if(setPlayerPrecision(p,DEFAULT_PLAYER_PRECISION)==ERR){
		free(p);
		return NULL;
	}
	
	p->sk_n = DEFAULT_SK_N + u_skills;
	
	p->ppoints = (short*)malloc(p->sk_n*sizeof(short));
	if(p->ppoints==NULL){
		free(p);
		return NULL;
	}
	
	for(int i=0; i<p->sk_n; i++) p->ppoints[i] = getSkillMaxPP(list,i);
	
	for(int i=0; i<ACTUAL_STATES_N; i++){
		p->states[i] = false;
		p->counters[i] =  TURNS_AFFLICTED;
	}

	return p;
}

/*health functions*/
short getPlayerHealth(player *p){
	if(p==NULL) return ERR;
	return p->health;
}
short getPlayerMaxHealth(player *p){
	if(p==NULL) return ERR;
	return p->max_health;
}
status setPlayerHealth(player *p, short health){
	if(p==NULL||health>MAX_PLAYER_HEALTH) return ERR;
	if(health<0) health = 0;
	if(health > getPlayerMaxHealth(p)) health = getPlayerMaxHealth(p);
	p->health = health;
	return OK;
}

/*state functions*/
status setPlayerState(player* p, short stindex){
	if(p==NULL||stindex<0||stindex>STATES_N) return ERR;
	
	switch(stindex){
		case NONE:
			return OK;
		case ATT_UP:
			if(setPlayerAttack(p,getPlayerAttack(p) + PD_ATTACK)==ERR) return ERR;
			return OK;
		case ATT_DOWN:
			if(setPlayerAttack(p,getPlayerAttack(p) - PD_ATTACK)==ERR) return ERR;
			return OK;
		case DEF_UP:
			if(setPlayerDefense(p,getPlayerDefense(p) + PD_DEFENSE)==ERR) return ERR;
			return OK;
		case DEF_DOWN:
			if(setPlayerDefense(p,getPlayerDefense(p) - PD_DEFENSE)==ERR) return ERR;
			return OK;
		case POISON:
			p->counters[POI] = TURNS_AFFLICTED;
			p->states[POI] = true;
			return OK;
		case BLIND:
			p->counters[BLI] = TURNS_AFFLICTED;
			p->states[BLI] = true;
			return OK;
		case MINDFUCKED:
			p->counters[MIN] = TURNS_AFFLICTED;
			p->states[MIN] = true;
			return OK;
		case PARAL:
			p->counters[PAR] = TURNS_AFFLICTED;
			p->states[PAR] = true;
			return OK;
		case CURED:
			removeAllPlayerStates(p);
			return OK;
		default:
			return ERR;
	}
	
	return OK;
}
void removeAllPlayerStates(player* p){
	if(p==NULL) return;
	for(int i=0; i<ACTUAL_STATES_N; i++){
		p->counters[i] = TURNS_AFFLICTED;
		p->states[i] = false;
	}
	return;
}
void removePlayerState(player* p, short stindex){
	if(p==NULL||stindex<0||stindex>STATES_N) return;
	switch(stindex){
		case POISON:
			p->counters[POI] = TURNS_AFFLICTED;
			p->states[POI] = false;
			break;
		case BLIND:
			p->counters[BLI] = TURNS_AFFLICTED;
			p->states[BLI] = false;
			break;
		case MINDFUCKED:
			p->counters[MIN] = TURNS_AFFLICTED;
			p->states[MIN] = false;
			break;
		case PARAL:
			p->counters[PAR] = TURNS_AFFLICTED;
			p->states[PAR] = false;
			break;
		default:
			return;
	}
	return;
}

bool isPlayerPoisoned(player* p){
	if(p==NULL) return false;
	return p->states[POI];
}
bool isPlayerBlind(player* p){
	if(p==NULL) return false;
	return p->states[BLI];
}
bool isPlayerMindfucked(player* p){
	if(p==NULL) return false;
	return p->states[MIN];
}
bool isPlayerParalized(player* p){
	if(p==NULL) return false;
	return p->states[PAR];
}

short getPlayerSkillN(player* p) {
	if(p==NULL) return ERR;
	return p->sk_n;
}

/*number of turns the effect still has*/
short turnsLeftPlayer(player* p, short stindex){
	if(p==NULL||stindex<0||stindex>ACTUAL_STATES_N-1) return ERR;
	return p->counters[stindex];
}
status countDownPlayer(player* p, short stindex){
	if(p==NULL||stindex<0||stindex>ACTUAL_STATES_N-1) return ERR;
	p->counters[stindex] = p->counters[stindex] - 1;
	return OK;
}

/*ppoints functions*/
short getPlayerPPoints(player* p, short sindex){
	if(p==NULL||sindex<0||sindex > p->sk_n) return ERR;
	return p->ppoints[sindex];
}
status setPlayerPPoints(player *p, short sindex, short points){
	if(p==NULL||sindex<0||sindex>SKILL_N-1||points<0||points>MAX_PPOINTS) return ERR;
	p->ppoints[sindex] = points;
	return OK;
}

/*Attack-defense-precision functions*/
float getPlayerAttack(player *p){
	if(p==NULL) return -1;
	return p->attack;
}
float getPlayerDefense(player *p){
	if(p==NULL) return -1;
	return p->defense;
}
status setPlayerAttack(player *p, float value){
	if(p==NULL) return ERR;
	if(value<=0) value=PLAYER_MIN_VALUE;
	p->attack = value;
	return OK;	
}
status setPlayerDefense(player *p, float value){
	if(p==NULL) return ERR;
	if(value<=0) value=PLAYER_MIN_VALUE;
	p->defense = value;
	return OK;
}
float getPlayerPrecision(player *p){
	if(p==NULL) return -1;
	return p->precision;
}
status setPlayerPrecision(player *p, float value){
	if(p==NULL) return ERR;
	if(value<=0) value=PLAYER_MIN_VALUE;
	p->precision = value;
	return OK;
}

/*frees player memory*/
void destroyPlayer(player *p){
	if(p==NULL) return;
	free(p->ppoints);
	free(p);
}
