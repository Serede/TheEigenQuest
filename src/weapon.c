#include "weapon.h"

/* PUBLIC DEFINITIONS */

struct _Weapon {
	element type; /* Weapon type */
	short ammo, max_ammo; /* Current and max_ammo */
	short bullet_speed; /* Bullet speed */
	bool unlocked; /* Whether it is unlocked */
	char name[W_MAX_NAME_LEN], desc[W_MAX_DESC_LEN]; /* Weapon metadata */
};


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Loads array of weapons */
Weapon *initWeapons(){
	Weapon *w;
	FILE *file;
	int i;
	short ammo, max_ammo, bullet_speed;
	char name[W_MAX_NAME_LEN], desc[W_MAX_DESC_LEN], buf[W_MAX_NAME_LEN+W_MAX_DESC_LEN+1];
	char *start, *end;
	struct timespec ts;

	/* Open weapons file */
	file = fopen(WEAPONS_FILE,"r");
	if(file==NULL) return NULL;

	/* Allocate array of weapons */
	w=(Weapon*)malloc(ELEMENTS_N*sizeof(Weapon));
	if(w==NULL) {
		fclose(file);
		return NULL;
	}

	/* Read weapons from file */
	for(i=0;i<ELEMENTS_N;i++){
		if(fgets(buf,W_MAX_NAME_LEN+W_MAX_DESC_LEN+1,file)==NULL) {
			free(w);
			fclose(file);
			return NULL;
		}

		for(start=end=buf;*end!=';';end++) {
			if(*end==EOS) {
				free(w);
				fclose(file);
				return NULL;
			}
		}
		*end=EOS;
		strcpy(name, start);

		for(start=++end;*end!=';';end++) {
			if(*end==EOS) {
				free(w);
				fclose(file);
				return NULL;
			}
		}
		*end=EOS;
		strcpy(desc, start);

		if(name==NULL||desc==NULL){
			fclose(file);
			free(w);
			return NULL;
		}

		if(sscanf(++end,"%hd/%hd:%hd\n",&ammo,&max_ammo,&bullet_speed)!=3) {
			fclose(file);
			free(w);
			return NULL;
		}

		w[i].type = i;
		w[i].ammo = ammo;
		w[i].max_ammo = max_ammo;
		w[i].bullet_speed = bullet_speed;
		w[i].unlocked = false;
		strcpy(w[i].name,name);
		strcpy(w[i].desc,desc);
	}
	fclose(file);

	/* Seed for getWeaponNeedAmmo */
	clock_gettime(CLOCK_REALTIME,&ts);
	srandom(ts.tv_nsec ^ ts.tv_sec);

	return w;
}

/* Returns weapon current ammo */
short getWeaponAmmo(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return -1;
	return windex==0? 1 : w[windex].ammo;
}

/* Returns windex of weapon that needs ammo */
short getWeaponNeedAmmo(Weapon *w) {
	short i, j;
	short loads[W_DICE_SIDES+1] = {0,W_DICE_LOADS};
	short accums[W_DICE_SIDES+1] = {0};
	short result, threshold, max, winner;

	/* Roll the dice until meeting resolution */
	for(max=-1,winner=0,i=0;i<W_DICE_RES;i++) {
		/* Generate a random percentage */
		result = (short)(100.0*random()/(RAND_MAX+1.0));

		/* Check winner side */
		for(threshold=0,j=0;j<W_DICE_SIDES;j++) {
			threshold += loads[j+1];
			if(result<threshold) break;
		}

		/* Accumulate result */
		if((!j || (w[j].unlocked && w[j].ammo)) && (++accums[j+1]>max)) {
			max = accums[j+1];
			winner = j;
		}
	}

	return winner;
}

/* Increases or decreases weapon ammo by given amount */
status changeWeaponAmmo(Weapon *w, short windex, short amount) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return ERR;
	if(!windex) return OK;
	w[windex].ammo+=amount;
	if(w[windex].ammo<0) w[windex].ammo=0;
	else if(w[windex].ammo>w[windex].max_ammo) w[windex].ammo = w[windex].max_ammo;
	return OK;
}

/* Returns weapon bullet speed */
short getWeaponBulletSpeed(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return -1;
	return w[windex].bullet_speed;
}

/* Checks whether a weapon is unlocked */
bool isWeaponUnlocked(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return false;
	return w[windex].unlocked;
}


/* Sets weapon as unlocked */
status unlockWeapon(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return ERR;
	w[windex].unlocked = true;
	return OK;
}

/* Returns pointer to weapon name */
char *getWeaponName(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return NULL;
	return w[windex].name;
}

/* Returns pointer to weapon description */
char *getWeaponDesc(Weapon *w, short windex) {
	if(w==NULL||windex<0||windex>=ELEMENTS_N) return NULL;
	return w[windex].desc;
}

/* Frees array of weapons */
void destroyWeapons(Weapon *w){
	if(w==NULL) return;
	free(w);
}
