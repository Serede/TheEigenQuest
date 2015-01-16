#include "platform.h"

/* PUBLIC DEFINITIONS */

struct _Platform {
	World *world; /* Platform world */
	pthread_mutex_t world_m; /* World mutex */
	pthread_t screen, collision, gravity, speed, stance, bullets, momentum, touch, kill, input; /* Threads */
	pthread_mutex_t screen_m, collision_m, gravity_m, speed_m, stance_m, bullets_m, momentum_m, touch_m, kill_m, input_m;  /* Threads mutexes */
};


/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

/** Threads **/

/* Screen refresh thread */
void *pfScreen(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->screen_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(printWorld(PF_DEFAULT_SCREEN,pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_SCREEN_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->screen_m)!=0) return NULL;
	return pf_t;
}

/* Collision computing thread */
void *pfCollision(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->collision_m)!=0) return NULL;
	status = worldGetStatus(pf->world);
	/* While world is up */
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldUpdateCollision(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_COLLISION_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->collision_m)!=0) return NULL;
	return pf_t;
}

/* Gravity thread */
void *pfGravity(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->gravity_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldUpdateGravity(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_GRAVITY_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->gravity_m)!=0) return NULL;
	return pf_t;
}

/* Speed computing thread */
void *pfSpeed(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->speed_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldUpdateSpeed(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_SPEED_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->speed_m)!=0) return NULL;
	return pf_t;
}

/* Stance updating thread */
void *pfStance(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->stance_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldUpdateStance(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_STANCE_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->stance_m)!=0) return NULL;
	return pf_t;
}

/* Bullet updating thread */
void *pfBullets(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->bullets_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldUpdateBullets(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_BULLETS_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->bullets_m)!=0) return NULL;
	return pf_t;
}

/* Momentum applying thread */
void *pfMomentum(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->momentum_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldApplyMomentum(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_MOMENTUM_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->momentum_m)!=0) return NULL;
	return pf_t;
}

/* Touching thread */
void *pfTouch(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->touch_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldApplyTouch(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_TOUCH_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->touch_m)!=0) return NULL;
	return pf_t;
}

/* Enemy killing thread */
void *pfKill(void *pf_t) {
	joukyou status;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->kill_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		if(worldApplyKill(pf->world)==ERR) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		if(status==YAME) break;
		usleep((int)(1/(double)PF_KILL_RATIO*SECS));
	}
	if(pthread_mutex_unlock(&pf->kill_m)!=0) return NULL;
	return pf_t;
}

/* Input thread */
void *pfInput(void *pf_t) {
	int ch;
	joukyou status=0;
	Platform *pf=pf_t; /* Recover type */
	if(pf_t==NULL) return NULL;
	/* Lock thread mutex */
	if(pthread_mutex_lock(&pf->input_m)!=0) return NULL;
	/* While world is up */
	status = worldGetStatus(pf->world);
	while(status!=YAME && status!=TASSEI && status!=SHINDA) {
		if(status==GENKI) {
			fflush(stdin);
			ch = getch();
			if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
			switch(ch) {
				case KEY_LEFT:
					worldPlayerWalk(pf->world,LEFT);
					break;
				case KEY_RIGHT:
					worldPlayerWalk(pf->world,RIGHT);
					break;
				case ' ':
					worldPlayerJump(pf->world);
					break;
				case 'z':
					worldWeaponSwitch(pf->world);
					break;
				case 'x':
					worldWeaponShoot(pf->world);
					break;
				case 'q': /* Esc */
					pauseWorld(pf->world);
					break;
			}
			if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
		}
		if(pthread_mutex_lock(&pf->world_m)!=0) return NULL;
		status = worldGetStatus(pf->world);
		if(pthread_mutex_unlock(&pf->world_m)!=0) return NULL;
	}
	if(pthread_mutex_unlock(&pf->input_m)!=0) return NULL;
	return pf_t;
}


/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Initializes and starts platform */
Platform *startPlatform(level lvl, short unlocked_weapons) {
	Platform *pf;

	/* Allocate platform */
	pf=(Platform*)malloc(sizeof(Platform));
	if(pf==NULL) return NULL;
	
	/* Allocate world */
	pf->world = initWorld(lvl,unlocked_weapons);
	if(pf->world==NULL) {
		free(pf);
		return NULL;
	}

	/* Initialize mutexes */
	if(pthread_mutex_init(&pf->world_m,NULL)!=0) {
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->screen_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->collision_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->gravity_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->speed_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->stance_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->bullets_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->momentum_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->touch_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->kill_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	if(pthread_mutex_init(&pf->input_m,NULL)!=0) {
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start screen thread */
	if(pthread_create(&pf->screen,NULL,pfScreen,(void*)pf)!=0) {
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->screen)!=0) {
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start collision thread */
	if(pthread_create(&pf->collision,NULL,pfCollision,(void*)pf)!=0) {
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->collision)!=0) {
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start gravity thread */
	if(pthread_create(&pf->gravity,NULL,pfGravity,(void*)pf)!=0) {
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->gravity)!=0) {
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	
	/* Start speed thread */
	if(pthread_create(&pf->speed,NULL,pfSpeed,(void*)pf)!=0) {
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->speed)!=0) {
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start stance thread */
	if(pthread_create(&pf->stance,NULL,pfStance,(void*)pf)!=0) {
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->stance)!=0) {
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start bullets thread */
	if(pthread_create(&pf->bullets,NULL,pfBullets,(void*)pf)!=0) {
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->bullets)!=0) {
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start momentum thread */
	if(pthread_create(&pf->momentum,NULL,pfMomentum,(void*)pf)!=0) {
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->momentum)!=0) {
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start touch thread */
	if(pthread_create(&pf->touch,NULL,pfTouch,(void*)pf)!=0) {
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->touch)!=0) {
		pthread_cancel(pf->touch);
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start kill thread */
	if(pthread_create(&pf->kill,NULL,pfKill,(void*)pf)!=0) {
		pthread_cancel(pf->touch);
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->kill)!=0) {
		pthread_cancel(pf->kill);
		pthread_cancel(pf->touch);
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	/* Start input thread */
	if(pthread_create(&pf->input,NULL,pfInput,(void*)pf)!=0) {
		pthread_cancel(pf->kill);
		pthread_cancel(pf->touch);
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}
	usleep(PF_THREAD_TIMEOUT);
	if(pthread_detach(pf->input)!=0) {
		pthread_cancel(pf->input);
		pthread_cancel(pf->kill);
		pthread_cancel(pf->touch);
		pthread_cancel(pf->momentum);
		pthread_cancel(pf->bullets);
		pthread_cancel(pf->stance);
		pthread_cancel(pf->speed);
		pthread_cancel(pf->gravity);
		pthread_cancel(pf->collision);
		pthread_cancel(pf->screen);
		pthread_mutex_destroy(&pf->input_m);
		pthread_mutex_destroy(&pf->kill_m);
		pthread_mutex_destroy(&pf->touch_m);
		pthread_mutex_destroy(&pf->momentum_m);
		pthread_mutex_destroy(&pf->bullets_m);
		pthread_mutex_destroy(&pf->stance_m);
		pthread_mutex_destroy(&pf->speed_m);
		pthread_mutex_destroy(&pf->gravity_m);
		pthread_mutex_destroy(&pf->collision_m);
		pthread_mutex_destroy(&pf->screen_m);
		pthread_mutex_destroy(&pf->world_m);
		destroyWorld(pf->world);
		free(pf);
		return NULL;
	}

	return pf;
}

/* Waits for platform to stop and returns its*
 * exit code: -1:Error, 0:Success, 1:Dead, 2:Quit */
short waitForPlatform(Platform *pf) {
	if(pf==NULL) return -1;
	pthread_mutex_lock(&pf->input_m);
	pthread_mutex_lock(&pf->touch_m);
	pthread_mutex_lock(&pf->momentum_m);
	pthread_mutex_lock(&pf->bullets_m);
	pthread_mutex_lock(&pf->stance_m);
	pthread_mutex_lock(&pf->speed_m);
	pthread_mutex_lock(&pf->gravity_m);
	pthread_mutex_lock(&pf->collision_m);
	pthread_mutex_lock(&pf->screen_m);
	pthread_mutex_unlock(&pf->screen_m);
	pthread_mutex_unlock(&pf->collision_m);
	pthread_mutex_unlock(&pf->gravity_m);
	pthread_mutex_unlock(&pf->speed_m);
	pthread_mutex_unlock(&pf->stance_m);
	pthread_mutex_unlock(&pf->bullets_m);
	pthread_mutex_unlock(&pf->momentum_m);
	pthread_mutex_unlock(&pf->touch_m);
	pthread_mutex_unlock(&pf->input_m);
	usleep(9*PF_THREAD_TIMEOUT);
	switch(worldGetStatus(pf->world)) {
		case TASSEI: return 0;
		case SHINDA: return 1;
		case YAME: return 2;
	}
	return -1;
}

/* Destroys platform (must be stopped) */
void destroyPlatform(Platform *pf) {
	if(pf==NULL) return;
	pthread_mutex_destroy(&pf->input_m);
	pthread_mutex_destroy(&pf->touch_m);
	pthread_mutex_destroy(&pf->momentum_m);
	pthread_mutex_destroy(&pf->bullets_m);
	pthread_mutex_destroy(&pf->stance_m);
	pthread_mutex_destroy(&pf->speed_m);
	pthread_mutex_destroy(&pf->gravity_m);
	pthread_mutex_destroy(&pf->collision_m);
	pthread_mutex_destroy(&pf->screen_m);
	pthread_mutex_destroy(&pf->world_m);
	destroyWorld(pf->world);
	free(pf);
}
