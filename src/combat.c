#include "combat.h"

#define YOU 1
#define HIM 0

struct _combat_t{
	player* player;
	boss* boss;

	combat_skel* cs;

	char** messages;
};

struct _combat_skel{
	skill* skill_list;

	short boss_health_bar, player_health_bar;
	
	char** options;
	char** descriptions;
};

/*Inits turn based combat*/
status initCombat(short cmb, short u_skills, short max_health, bool* result){
	combat_t* combat;

	if(cmb<0||cmb>BOSS_N-1) return ERR;
	
	combat = (combat_t*)malloc(sizeof(combat_t));
	if(combat==NULL) return ERR;
	
	skill* skill_list;
	
	if((skill_list = loadSkills())==NULL){
		free(combat);
		return ERR;
	}
	
	combat->player = initPlayer(skill_list, u_skills, max_health);
	if(combat->player==NULL){
		free(combat);
		return ERR;
	}
	
	combat->boss = initBoss(cmb);
	if(combat->boss==NULL){
		destroyPlayer(combat->player);
		free(combat);
		return ERR;
	}
	
	if((combat->cs = initSkeleton(skill_list, combat->player, combat->boss))==NULL){
		destroyPlayer(combat->player);
		destroyBoss(combat->boss);
		free(combat);
		return ERR;
	}
	
	combat->messages = loadMessages(cmb);
	if(combat->messages==NULL){
		destroyPlayer(combat->player);
		destroyBoss(combat->boss);
		destroySkeleton(combat->cs);
		free(combat);
		return ERR;
	}
	
	if(initCombatInterface()==ERR){
		destroyPlayer(combat->player);
		destroyBoss(combat->boss);
		destroySkeleton(combat->cs);
		unloadMessages(combat->messages);
		free(combat);
		return ERR;
	}
	
	if(combatLoop(combat->cs, combat->player, combat->boss, combat->messages, result)==ERR){
		destroyPlayer(combat->player);
		destroyBoss(combat->boss);
		destroySkeleton(combat->cs);
		unloadMessages(combat->messages);
		free(combat);
		return ERR;
	}
	
	char t[1024];
	sprintf(t,WIN_MSG,getBossName(combat->boss));
	if(*result==true) setTextBottom(t);
	else setTextBottom(LOSE_MSG);
	
	destroyCombatInterface();
	destroyPlayer(combat->player);
	destroyBoss(combat->boss);
	destroySkeleton(combat->cs);
	unloadMessages(combat->messages);
	free(combat);
	
	return OK;	
}

bool startCombat(stage cmb, short u_skills, short max_health){
	if(cmb<0||cmb>BOSS_N) return false;
	
	bool result;
	if(initCombat(cmb,u_skills,max_health,&result)==ERR) return false;
	
	return result;
}

/*Load messages that the combat will use*/
char** loadMessages(stage cmb){
	if(cmb<0||cmb>BOSS_N-1) return NULL;
	char** messages = (char**)malloc(BOSS_MSG_N*sizeof(char*));
	if(messages==NULL) return NULL;
	
	for(int i=0; i<BOSS_MSG_N; i++){
		messages[i] = (char*)malloc(MAX_TEXT_MSG*sizeof(char));
		if(messages[i]==NULL){
			for(int j=i-1; j>=0; j--) free(messages[i]);
			free(messages);
			return NULL;
		}
		
		/*This is probably not the best way to go around this, but hey, it's 2:00AM. It can't go wrong*/
		/*The switch-switch of death*/
		switch(cmb){
			case BILIAR:
				switch(i){
					case START:
						strcpy(messages[i],BILIAR_START_MSG);
						break;
					case WINS:
						strcpy(messages[i],BILIAR_WINS_MSG);
						break;
					case LOSES:
						strcpy(messages[i],BILIAR_LOSES_MSG);
						break;
					case MID_FIGHT:
						strcpy(messages[i],BILIAR_MID_FIGHT_MSG);
						break;
					case END:
						strcpy(messages[i],BILIAR_END_MSG);
						break;
					case RAN:
						strcpy(messages[i],BILIAR_RAND_MSG);
						break;
					default:
						strcpy(messages[i],ERR_MSG);
						break;
				}
				break;
			case CYCLOPS:
				switch(i){
					case START:
						strcpy(messages[i],CYCLOPS_START_MSG);
						break;
					case WINS:
						strcpy(messages[i],CYCLOPS_WINS_MSG);
						break;
					case LOSES:
						strcpy(messages[i],CYCLOPS_LOSES_MSG);
						break;
					case MID_FIGHT:
						strcpy(messages[i],CYCLOPS_MID_FIGHT_MSG);
						break;
					case END:
						strcpy(messages[i],CYCLOPS_END_MSG);
						break;
					case RAN:
						strcpy(messages[i],CYCLOPS_RAND_MSG);
						break;
					default:
						strcpy(messages[i],ERR_MSG);
						break;
				}
				break;
			case IRONMAN:
				switch(i){
					case START:
						strcpy(messages[i],IRONMAN_START_MSG);
						break;
					case WINS:
						strcpy(messages[i],IRONMAN_WINS_MSG);
						break;
					case LOSES:
						strcpy(messages[i],IRONMAN_LOSES_MSG);
						break;
					case MID_FIGHT:
						strcpy(messages[i],IRONMAN_MID_FIGHT_MSG);
						break;
					case END:
						strcpy(messages[i],IRONMAN_END_MSG);
						break;
					case RAN:
						strcpy(messages[i],IRONMAN_RAND_MSG);
						break;
					default:
						strcpy(messages[i],ERR_MSG);
						break;
				}
				break;
			case CUERVS:
				switch(i){
					case START:
						strcpy(messages[i],CUERVS_START_MSG);
						break;
					case WINS:
						strcpy(messages[i],CUERVS_WINS_MSG);
						break;
					case LOSES:
						strcpy(messages[i],CUERVS_LOSES_MSG);
						break;
					case MID_FIGHT:
						strcpy(messages[i],CUERVS_MID_FIGHT_MSG);
						break;
					case END:
						strcpy(messages[i],CUERVS_END_MSG);
						break;
					case RAN:
						strcpy(messages[i],CUERVS_RAND_MSG);
						break;
					default:
						strcpy(messages[i],ERR_MSG);
						break;
				}
				break;
			default:
				strcpy(messages[i],ERR_MSG);
				break;
		}
		/*It had to be either this or a char*** messages*/
	}
	return messages;
}

/*Unload messages that the combat will use*/
void unloadMessages(char** messages){
	if(messages==NULL) return;
	for(int i=0; i<BOSS_MSG_N; i++) free(messages[i]);
	free(messages);
	return;
}

/*Returns an array of strings with the descriptions to the*/
char** loadDescriptions(skill* skill_list){
	if(skill_list==NULL) return NULL;
	char** descriptions = (char**)malloc(SKILL_N*sizeof(char*));
	if(descriptions==NULL) return NULL;
	for(int i=0; i<SKILL_N; i++){
		descriptions[i] = (char*)malloc(MAX_SDESC*sizeof(char));
		if(descriptions[i]==NULL) return NULL;
		sprintf(descriptions[i],"%s\n\n"\
				"Target: %s\n"\
				"Damage: %hd (%hd%s Accuracy)\n"\
				"Status: %s (%hd%s Accuracy)\n",\
				getSkillDesc(skill_list,i),\
				getSkillTarget(skill_list,i)? "Jay Phi":"Enemy",\
				getSkillDamage(skill_list,i),\
				getSkillAttackPrecision(skill_list,i),"%%",\
				getStateString(getSkillEffect(skill_list,i)),\
				getSkillStatePrecision(skill_list,i),"%%"
		       );
		/*strcpy(descriptions[i],getSkillDesc(skill_list,i));*/
	}
	return descriptions;
}

/*Initialize the data needed for the combat interface*/
combat_skel* initSkeleton(skill* skill_list, player* p, boss* b){
	if(skill_list==NULL||p==NULL||b==NULL) return NULL;

	combat_skel* cs = (combat_skel*)malloc(sizeof(combat_skel));
	if(cs==NULL) return NULL;

	cs->player_health_bar = getPlayerHealth(p);
	if(cs->player_health_bar==ERR){
		free(cs);
		return NULL;
	}

	cs->boss_health_bar = getBossHealth(b);
	if(cs->boss_health_bar == ERR){
		free(cs);
		return NULL;
	}

	cs->skill_list = skill_list;

	if((cs->options = loadOptions(cs->skill_list,p))==NULL){
		unloadSkills(cs->skill_list);
		free(cs);
		return NULL;
	}

	if((cs->descriptions = loadDescriptions(skill_list))==NULL){
		unloadSkills(cs->skill_list);
		free(cs);
		return NULL;
	}

	return cs;
}

/*Smashes the skeleton to bonemeal*/
void destroySkeleton(combat_skel* cs){
	if(cs==NULL) return;
	unloadSkills(cs->skill_list);
	free(cs);
	return;
}

/*Loads the combat options*/
char** loadOptions(skill* s_list, player* p){
	if(s_list==NULL||p==NULL) return NULL;

	/*Possible input argument for the module? Yes, but this functions was already written at that point ¯\_(ツ)_/¯*/
	int n = getPlayerSkillN(p);
	if(n<=0) return NULL;
	char** options=(char**)malloc(n*sizeof(char*));
	if(options==NULL) return NULL;

	/*skills unlocked are in order, for sanity's sake*/

	char* name;
	/*Load the names of the skills*/
	for(int i=0;i<n;i++){
		name=getSkillName(s_list,i);
		options[i]=(char*)malloc(SKILL_MAX_NAME*sizeof(char));
		strcpy(options[i],name);
	}
	return options;
}

/*writes the number of points available to each skill*/
char** writePPIntoOptions(player* p, char** messages){
	int n = getPlayerSkillN(p);
	if(n<=0) return NULL;
	char** updated = (char**)malloc(n*sizeof(char*));
	if(updated==NULL) return NULL;
	
	for(int i=0;i<n;i++){
		updated[i]=(char*)malloc(SKILL_MAX_NAME*sizeof(char));
		sprintf(updated[i],"%s (%hd)",messages[i],getPlayerPPoints(p,i));
	}
	return updated;
}

/*Unloads options*/
void unloadOptions(char** options, short howmany){
	int i=0;
	if(options==NULL) return;
	for(int i=0; i<howmany;i++) free(options[i]);
	free(options);
}

/*Combat functions*/

/*ROLL THAT DICE*/
int rickRoll(int inf, int sup){
	if(sup<inf||inf<0||sup<0) return -1;
	int dice = inf + (int)(((double)sup-inf+1)*random()/(RAND_MAX+1.0));
	return dice;
}

/*Function to set the information plaques about the fighters*/
void updateBoxes(player* p, boss* b){
  char color='b';
  int health, max_health;
  double ratio;
  if(p==NULL||b==NULL) return;

	char player_text[STRING_MAX_LENGTH], boss_text[STRING_MAX_LENGTH], general_text[STRING_MAX_LENGTH];
	
	sprintf(boss_text,"%s",getBossName(b));
	if(isBossMindfucked(b))  sprintf(boss_text,"%s $em[%s]$dm",boss_text,STRING_MINDFUCKED);
	if(isBossBlind(b)) sprintf(boss_text,"%s $ek[%s]$dk",boss_text,STRING_BLIND);
	if(isBossParalized(b)) sprintf(boss_text,"%s $ey[%s]$dy",boss_text,STRING_PARALIZED);
	if(isBossPoisoned(b)) sprintf(boss_text,"%s $eg[%s]$dg",boss_text,STRING_POISON);
	health = getBossHealth(b);
	max_health=getBossMaxHealth(b);
	ratio = health / (double) max_health;
	color = ratio>GREEN_THRESHOLD? 'g': 
	        ratio>YELLOW_THRESHOLD? 'y':
	        'r';
	  sprintf(boss_text,"%s\t\t($e%c%hd$d%c/%hd)\t\t\t",boss_text,color,  health, color, max_health);
	setTextEnemy(boss_text);

	sprintf(player_text,"%s",PLAYER_NAME);
	if(isPlayerMindfucked(p)) sprintf(player_text,"%s $em[%s]$dm",player_text,STRING_MINDFUCKED);
	if(isPlayerBlind(p)) sprintf(player_text,"%s $ek[%s]$dk",player_text,STRING_BLIND);
	if(isPlayerParalized(p)) sprintf(player_text,"%s $ey[%s]$dy",player_text,STRING_PARALIZED);
	if(isPlayerPoisoned(p)) sprintf(player_text,"%s $eg[%s]$dg",player_text,STRING_POISON);

	health = getPlayerHealth(p);
	max_health = getPlayerMaxHealth(p);
	ratio = health / (double) max_health;
	color = ratio>GREEN_THRESHOLD? 'g': 
	        ratio>YELLOW_THRESHOLD? 'y':
	        'r';
	sprintf(player_text,"%s\t\t($e%c%hd$d%c/%hd)\t\t\t",player_text,color,  health, color, max_health);
	setTextPlayer(player_text);
	
	setEnemyBar(getBossHealth(b),getBossMaxHealth(b));
	setPlayerBar(getPlayerHealth(p),getPlayerMaxHealth(p));
}

char *getPixmapPath(boss* b) {
	if(b==NULL)return "\0";
	switch(getBossId(b)) {
		case BILIAR:
			return BILIAR_PIXMAP;
		case CYCLOPS:
			return CYCLOPS_PIXMAP;
		case IRONMAN:
			return IRONMAN_PIXMAP;
		case CUERVS:
			return CUERVS_PIXMAP;
		default:
			return "\0";
	}
}

/*Combat loop*/
status combatLoop(combat_skel* cs, player* p, boss* b, char** messages, bool* result){
	if(cs==NULL||p==NULL||b==NULL) return ERR;
	bool fighting;
	short p_choice, b_choice, tryagain, turn;
	char text[STRING_MAX_LENGTH];
	char** updatedOptions;
	turn = 0;
	char t[1024];
	struct timespec ts;
	
	updateBoxes(p,b);
	
	if(setPlayerPixmap(PLAYER_PIXMAP)==ERR) mvprintw(20, 20, "Fail Player PM");
	//We have to control which pixmap to put via the boss/combat id, but for now:
	if(setEnemyPixmap(getPixmapPath(b))==ERR) mvprintw(20, 20, "Fail Boss PM");
	
	sprintf(text,BATTLE_START_MSG,getBossTitle(b));
	setTextBottom(text);

	/* Seed for rickRoll */
	clock_gettime(CLOCK_REALTIME,&ts);
	srandom(ts.tv_nsec ^ ts.tv_sec);
	
	fighting = true;
	
	/*here we go*/
	while(fighting){
		
		if(turn==0) setTextBottom(messages[START]);
		
		updatedOptions = writePPIntoOptions(p,cs->options);
		
		do {
			tryagain = 0;
			p_choice = setTextDialog(getPlayerSkillN(p),DO_YOU_LOVE_FANSI_C,updatedOptions,cs->descriptions);
			if(p_choice==ERR){
				fighting = false;
				*result = false;
				return ERR;
			}
		  	if(playerAction(cs, p, b, p_choice, &tryagain)==ERR){
				fighting = false;
				*result = false;
				return ERR;
			}
		} while(tryagain==1);

		unloadOptions(updatedOptions,getPlayerSkillN(p));
		
		updateBoxes(p,b);
		
		if(getBossHealth(b)==0){
			fighting = false;
			*result = true;
		}

		/* I dont want Biliar to kill me after I kill him */
		if(fighting==true) {
			
			if(isBossPoisoned(b)){
				if(turnsLeftBoss(b,POI)==0){
					removeBossState(b,POISON);
					sprintf(text,BOSS_CURED_FROM_POISON_MSG,getBossName(b));
					setTextBottom(text);
				}
				else countDownBoss(b,POI);
			}
			
			if(isBossBlind(b)){
				if(turnsLeftBoss(b,BLI)==0){
					removeBossState(b,BLIND);
					setBossPrecision(b,NOT_BLIND_PRECISION);
					sprintf(text,BOSS_CURED_FROM_BLIND_MSG,getBossName(b));
					setTextBottom(text);
				}
				else {
					countDownBoss(b,BLI);
					setBossPrecision(b,BLIND_PRECISION);
				}
			}
			
			if(isBossMindfucked(b)){
				if(turnsLeftBoss(b,MIN)==0){
					removeBossState(b,MINDFUCKED);
					sprintf(text,BOSS_CURED_FROM_MINDFUCKED_MSG,getBossName(b));
					setTextBottom(text);
				}
				else countDownBoss(b,MIN);
			}
			
			if(isBossParalized(b)){
				if(turnsLeftBoss(b,PAR)==0){
					removeBossState(b,PARAL);
					sprintf(text,BOSS_CURED_FROM_PARALIZED_MSG,getBossName(b));
					setTextBottom(text);
				}
				else countDownBoss(b,PAR);
			}
			
			updateBoxes(p,b);

			b_choice = setBossChoice(b);
			if(bossAction(cs, b,p, b_choice)==ERR) setTextBottom(ERR_MSG);

			updateBoxes(p,b);


			if(isPlayerPoisoned(p)){
				if(turnsLeftPlayer(p,POI)==0){
					removePlayerState(p,POISON);
					setTextBottom(YOU_CURED_FROM_POISON_MSG);
				}
				else countDownPlayer(p,POI);
			}
			
			if(isPlayerBlind(p)){
				if(turnsLeftPlayer(p,BLI)==0){
					removePlayerState(p,BLIND);
					setPlayerPrecision(p,NOT_BLIND_PRECISION);
					setTextBottom(YOU_CURED_FROM_BLIND_MSG);
				}
				else {
					countDownPlayer(p,BLI);
					setPlayerPrecision(p,BLIND_PRECISION);
				}
			}
			
			if(isPlayerMindfucked(p)){
				if(turnsLeftPlayer(p,MIN)==0){
					removePlayerState(p,MINDFUCKED);
					setTextBottom(YOU_CURED_FROM_MINDFUCKED_MSG);
				}
				else countDownPlayer(p,MIN);
			}
			
			if(isPlayerParalized(p)){
				if(turnsLeftPlayer(p,PAR)==0){
					removePlayerState(p,PARAL);
					setTextBottom(YOU_CURED_FROM_PARALIZED_MSG);
				}
				else countDownPlayer(p,PAR);
			}

			updateBoxes(p,b);
		}

		if(isBossPoisoned(b)){
			setBossHealth(b,getBossHealth(b)*POISON_DAMAGE);
			setTextBottom(BOSS_SUFFERED_POISON_DAMAGE_MSG);
		}
		if(isPlayerPoisoned(p)){
			setPlayerHealth(p,getPlayerHealth(p)*POISON_DAMAGE);
			setTextBottom(YOU_SUFFERED_POISON_DAMAGE_MSG);
		}

		updateBoxes(p,b);

		if(cs->player_health_bar<=0){
			fighting = false;
			*result = false;		
		}
		
		if(getBossHealth(b)==0){
		  fighting = false;
		  *result = true;
		}
		if(getBossHealth(b) < (short)HALF_LIFE(b) && getBossHealth(b) > (short)QUARTER_LIFE(b)) setTextBottom(messages[MID_FIGHT]);
		
		else if(getBossHealth(b) < (short)QUARTER_LIFE(b) && getBossHealth(b) != 0) setTextBottom(messages[END]);
		
		else if(rickRoll(0,5)==0) setTextBottom(messages[RAN]);

		turn++;
	}

	if(*result==true) setTextBottom(messages[LOSES]);
	else setTextBottom(messages[WINS]);
	
	return OK;
}

/*Actions attempt to use a skill from the attacker and
 updates the fighters fields. They also set the bottom
 text of the interface at each point*/

/*Something like the boss AI*/
short setBossChoice(boss* b){
	short i, min, max;
	if(b==NULL) return ERR;

	for(min=SKILL_N,max=-1,i=0;i<getBossSkillN(b);i++) {
		if(getBossSkills(b)[i]<min) min = getBossSkills(b)[i];
		if(getBossSkills(b)[i]>max) max = getBossSkills(b)[i];
	}
	if(min==SKILL_N||max==-1) return -1;

	return rickRoll(min,max);
}


void printDamage(short damage, short target, char* name){
	char text[STRING_MAX_LENGTH];
	if(target==YOU){
		if(damage > 0){
			sprintf(text,YOU_SUFFERED_DAMAGE_MSG,damage);
			setTextBottom(text);
		}
		else{
			sprintf(text,YOU_HEALED_MSG,-damage);
			setTextBottom(text);
		}
	}
	else {
		if(damage > 0){
			sprintf(text,BOSS_SUFFERED_DAMAGE_MSG,name,damage);
			setTextBottom(text);
		}
		else{
			sprintf(text,BOSS_HEALED_MSG,name);
			setTextBottom(text);
		}
	}
}

void printEffect(state effect, short target, char* name){
	char text[STRING_MAX_LENGTH];
	switch(effect){
		case NONE:
			return;
		case ATT_UP:
			if(target==YOU) sprintf(text,ATT_UP_MSG,PLAYER_NAME);
			else sprintf(text,ATT_UP_MSG,name);
			setTextBottom(text);
			return;
		case DEF_UP:
			if(target==YOU) sprintf(text,DEF_UP_MSG,PLAYER_NAME);
			else sprintf(text,DEF_UP_MSG,name);
			setTextBottom(text);
			return;
		case ATT_DOWN:
			if(target==YOU) sprintf(text,ATT_DOWN_MSG,PLAYER_NAME);
			else sprintf(text,ATT_DOWN_MSG,name);
			setTextBottom(text);
			return;
		case DEF_DOWN:
			if(target==YOU) sprintf(text,DEF_DOWN_MSG,PLAYER_NAME);
			else sprintf(text,DEF_DOWN_MSG,name);
			setTextBottom(text);
			return;
		case BLIND:
			if(target==YOU) sprintf(text,BLIND_MSG,PLAYER_NAME);
			else sprintf(text,BLIND_MSG,name);
			setTextBottom(text);
			return;
		case POISON:
			if(target==YOU) sprintf(text,POISON_MSG,PLAYER_NAME);
			else sprintf(text,POISON_MSG,name);
			setTextBottom(text);
			return;
		case MINDFUCKED:
			if(target==YOU) sprintf(text,MINDFUCK_MSG,PLAYER_NAME);
			else sprintf(text,MINDFUCK_MSG,name);
			setTextBottom(text);
			return;
		case PARAL:
			if(target==YOU) sprintf(text,PARALIZED_MSG,PLAYER_NAME);
			else sprintf(text,PARALIZED_MSG,name);
			setTextBottom(text);
			return;
		default:
			return;
	}
}

/*Action from player*/
status playerAction(combat_skel* cs, player* p, boss* b, short sindex, short* tryagain){
	if(cs==NULL||p==NULL||b==NULL||sindex<0||sindex > getPlayerSkillN(p))
		return ERR;
	
	short attk = 0, mind_var = 1, ppoints = getPlayerPPoints(p,sindex);
	state new_state = NONE;
	char text[MAX_TEXT_MSG];
	bool _attack =  false, _inflict = false, att_failed = false;

	cs->player_health_bar = getPlayerHealth(p);
	cs->boss_health_bar = getBossHealth(b);

	if(ppoints < 1){
		setTextBottom(NO_PPOINTS_MSG);
		*tryagain = 1;
		return OK;
	}
	
	sprintf(text,ACTION_ATTEMPT_MSG,PLAYER_NAME,getSkillName(cs->skill_list,sindex));
	setTextBottom(text);
	
	if(setPlayerPPoints(p, sindex, ppoints-1)==ERR) return ERR;
	
	mind_var = 1;
	if(isPlayerMindfucked(p)){
		if(rickRoll(1,100) > 50) mind_var = 0;
		else mind_var = 2;
	}

	attk = attack(cs->skill_list,getPlayerPrecision(p),sindex,&att_failed);
	new_state = inflict(cs->skill_list,getPlayerPrecision(p),sindex);
	if(new_state==NONE) _inflict = false;
	else _inflict = true;

	switch(getSkillTarget(cs->skill_list,sindex)){
		case SELF:
			attk = getPlayerAttack(p) * attk * mind_var;
			if(attk==0) _attack = false;
			else _attack = true;

			if(att_failed==true) setTextBottom(ATT_FAILED_MSG);
			
			else {
				if(isPlayerParalized(p))
					if(rickRoll(0,100) > PARAL_PROBABILITY){
						sprintf(text,PARALIZED_FAIL_MSG,PLAYER_NAME);
						setTextBottom(text);
						break;
					}
				if(_attack){
					cs->player_health_bar -= attk;
					if(setPlayerHealth(p,cs->player_health_bar)==ERR) return ERR;
					printDamage(attk, YOU, getBossName(b));
				}
				if(_inflict && mind_var!=0){
					if(setPlayerState(p,new_state)==ERR) return ERR;
					if(new_state==CURED) setTextBottom(YOU_CURED_MSG);
					else printEffect(new_state, YOU, getBossName(b));
				}
			}
			if(_attack==false && _inflict==false) setTextBottom(ATT_FAILED_MSG);

			break;
		case ENEMY:

			attk = getPlayerAttack(p) * attk * mind_var / getBossDefense(b);
			if(attk==0) _attack = false;
			else _attack = true;
			
			if(att_failed==true) setTextBottom(ATT_FAILED_MSG);
			
			else {
				if(isPlayerParalized(p))
					if(rickRoll(1,100) > PARAL_PROBABILITY){
						sprintf(text,PARALIZED_FAIL_MSG,PLAYER_NAME);
						setTextBottom(text);
						break;
					}
				if(_attack){
					cs->boss_health_bar -= attk;
					if(setBossHealth(b,cs->boss_health_bar)==ERR) return ERR;
					printDamage(attk, HIM, getBossName(b));
				}
				if(_inflict && mind_var!=0){
					if(setBossState(b,new_state)==ERR) return ERR;
					printEffect(new_state, HIM, getBossName(b));
				}
			}
			if(_attack==false && _inflict==false) setTextBottom(ATT_FAILED_MSG);

			break;
		default:
			return ERR;
	}
	
	return OK;
}

/*Action from boss*/
status bossAction(combat_skel*cs, boss* b, player* p, short sindex){;
	char t[STRING_MAX_LENGTH];
	short attk = 0, mind_var = 1;
	state new_state = NONE;
	char text[MAX_TEXT_MSG];
	bool _attack = false, _inflict = false, att_failed = false;
	
	if(cs==NULL||p==NULL||b==NULL||sindex<0)
		return ERR;
	
	cs->player_health_bar = getPlayerHealth(p);
	cs->boss_health_bar = getBossHealth(b);
	sprintf(t,ACTION_ATTEMPT_MSG,getBossName(b),getSkillName(cs->skill_list,sindex));
	setTextBottom(t);
	
	mind_var = 1;
	if(isBossMindfucked(b)){
		if(rickRoll(1,100) > 50) mind_var = 0;
		else mind_var = 2;
	}

	attk = attack(cs->skill_list,getBossPrecision(b),sindex,&att_failed);
	new_state = inflict(cs->skill_list,getBossPrecision(b),sindex);
	if(new_state==NONE) _inflict = false;
	else _inflict = true;

	switch(getSkillTarget(cs->skill_list,sindex)){
		case SELF:

			attk = getBossAttack(b) * attk * mind_var;
			if(attk==0) _attack = false;
			else _attack = true;

			if(att_failed==true) setTextBottom(ATT_FAILED_MSG);

			else {
				if(isBossParalized(b))
					if(rickRoll(1,100) > PARAL_PROBABILITY){
						sprintf(text,PARALIZED_FAIL_MSG,getBossName(b));
						setTextBottom(text);
						break;
					}
				if(_attack){
					cs->boss_health_bar -= attk;
					if(setBossHealth(b,cs->boss_health_bar)==ERR) return ERR;
					printDamage(attk,HIM,getBossName(b));
				}
				if(_inflict && mind_var!=0){
					if(setBossState(b,new_state)==ERR) return ERR;
					if(new_state==CURED){
						sprintf(text,BOSS_CURED_MSG,getBossName(b));
						setTextBottom(text);
					}
					else printEffect(new_state, HIM, getBossName(b));
				}
			}

			if(_attack==false && _inflict==false && att_failed==false) setTextBottom(ATT_FAILED_MSG); //only the state fail

			break;
		case ENEMY:

			attk = getBossAttack(b) * attk * mind_var / getPlayerDefense(p);
			if(attk==0) _attack = false;
			else _attack = true;
						
			if(att_failed==true) setTextBottom(ATT_FAILED_MSG);

			else {
				if(isBossParalized(b))
					if(rickRoll(1,100) > PARAL_PROBABILITY){
						sprintf(text,PARALIZED_FAIL_MSG,getBossName(b));
						setTextBottom(text);
						break;
					}
				if(_attack){
					cs->player_health_bar -= attk;
					if(setPlayerHealth(p,cs->player_health_bar)==ERR) return ERR;
					printDamage(attk,YOU,getBossName(b));
				}
				if(_inflict && mind_var!=0){
					if(setPlayerState(p,new_state)==ERR) return ERR;
					printEffect(new_state,YOU,getBossName(b));
				}
			}

			if(_attack==false && _inflict==false && att_failed==false) setTextBottom(ATT_FAILED_MSG);

			break;
		default:
			return ERR;
	}
	
	return OK;
}

/*Returns the damage inflicted by an action*/
short attack(skill* s, float precision, short sindex, bool* failed){
	char t[1024];
	if(s==NULL||sindex<0||sindex>SKILL_N) return 0;
	
	short damage = getSkillDamage(s,sindex);
	
	if(damage==0){
		*failed = false;
		return 0;
	}
	
	int success = rickRoll(0,100);
	short probability = getSkillAttackPrecision(s,sindex);
	
	if(success==-1||success > precision*probability){
		*failed = true;
		return 0;
	}
	
	*failed = false;
	return damage;
}

/*Returns the state inflicted by an action*/
state inflict(skill* s, float precision, short sindex){
	if(s==NULL||sindex<0||sindex>SKILL_N) return NONE;
	
	state effect = getSkillEffect(s,sindex);
	if(effect==NONE) return NONE;
	
	/*char t[64];
	sprintf(t,"Effect: %hd",effect);
	setTextBottom(t);*/

	int success = rickRoll(0,100);

	/*sprintf(t,"(%hd) (%f)",success,precision*getSkillStatePrecision(s,sindex));
	setTextBottom(t);*/
	
	if(success==-1||success > precision*getSkillStatePrecision(s,sindex)) return NONE;

	return effect;
}

