#include "platform.h"
#include "combat.h"
#include "color.h"
#include "utils.h"
#include "types.h"

#define SCREEN_H 40
#define SCREEN_W 160
#define MAX_TEXT 24
#define SAVE_SLOTS 8
#define SAVE_ASK "[SAVE] Select slot (1-8): "
#define SAVE_FORMAT "save/teq%02d.sav"
#define SAVE_UW_0 1
#define SAVE_UW_1 2
#define SAVE_UW_2 3
#define SAVE_UW_3 4
#define SAVE_US_0 0
#define SAVE_MH_0 100
#define SAVE_US_1 2
#define SAVE_MH_1 130
#define SAVE_US_2 5
#define SAVE_MH_2 160
#define SAVE_US_3 8
#define SAVE_MH_3 200
#define SAVE_US_4 10
#define SAVE_MH_4 400

/* Garrido Mode */
#define GM_KEY 'g'
#define GM_UW 0
#define GM_US SKILL_N-DEFAULT_SK_N
#define GM_MH SAVE_MH_4
#define GM_PR 8
#define GM_MSG "Okaerinasai, Goshujin-sama (\u02d8\u00b3\u02d8)\u2764"

/* Title macros */
#define TITLE_TIMEOUT 50000
#define TITLE_DIR "texts/title"
#define TITLE_SFRAMES 60
#define TITLE_AFRAMES 30
#define TITLE_FRAMES 90
#define TITLE_FPS 30
#define TITLE_EXTRAS 2
#define TITLE_EXTRA_Y 16
#define TITLE_EXTRA_X 69
#define TITLE_MSG_Y 33
#define TITLE_MSG_X 23
#define TITLE_LICENSE_X 95
#define TITLE_MSG "PRESS ANY KEY TO START"
#define TITLE_LICENSE "Garrido Warriors (GNU GPL v2.0) 2014-2015"
#define TITLE_CP 49

/* Menu macros */
#define MENU_N 4
#define MENU_Y {17, 20, 23, 26}
#define MENU_X 33
#define MENU_SAVE_X 103
#define MENU_T {"New Game", "Load Game", "Tutorial", "Quit"}
#define MENU_CP 13
#define MENU_MSG "SELECT AN OPTION AND PRESS ENTER"

/* Map macros */
#define MAP_FILE "texts/map"
#define MAP_W 121
#define MAP_H 32
#define MAP_BUF 365
#define MAP_CP 49
#define LVL_N 4
#define LVL_T {"EPS", "Nitrogen Pass", "Faculty of Science", "Secret Math Library"}
#define LVL_Y 13
#define LVL_X {14, 31, 58, 90}
#define BSS_N 4
#define BSS_T {"Dr. Biliar", "Electric Cyclops", "Ironman", "CUERVS"}
#define BSS_Y 21
#define BSS_X {11, 30, 64, 98}
#define PROGRESS_Y 27
#define PROGRESS_X 14
#define MAP_SELECT_CP 13

/* Structure to keep the progress of the game */
typedef struct {
	short unlocked_weapons;
	short unlocked_skills;
	short max_health;
	short progress;
} savegame;

/* Structure to animate the title screen */
typedef struct {
	Pixmap *frames[TITLE_AFRAMES];
	Pixmap *extras[TITLE_EXTRAS];
	ColorPair *cp;
} animation;

/* Thread management public variables */
bool start;
pthread_mutex_t start_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t title_m = PTHREAD_MUTEX_INITIALIZER;

/* Thread to play title screen animation */
void *title(void *anim_t) {
	Pixmap *temp;
	bool stop;
	int i;

	/* Recover types */
	animation *anim = (animation*)anim_t;
	if(anim==NULL) return NULL;

	/* Lock thread mutex */
	if(pthread_mutex_lock(&title_m)!=0) return NULL;
	
	/* Loop until menu sends keypress */
	stop = !start;
	while(!stop) {
		/* Print each frame */
		for(i=0;!stop&&i<TITLE_FRAMES;i++) {
			/* Ask for control and break if start is true (keypress income) */
			if(pthread_mutex_lock(&start_m)!=0) return NULL;
			if(!start) break;

			/* Merge extras into frames */
			temp = mergePixmaps(anim->extras[(i/3)%TITLE_EXTRAS],\
				anim->frames[\
					/* First frames keep static */
					(i<TITLE_SFRAMES? 0:i%TITLE_SFRAMES)\
				],\
				anim->cp,false);
			if(temp==NULL) return NULL;

			/* Print merged pixmap */
			printPixmap(stdscr,temp,false);

			/* Destroy temporal pixmap for merge */
			destroyPixmap(temp);

			/* Give control up and break if needed */
			if(pthread_mutex_unlock(&start_m)!=0) return NULL;
			if(!start) break;

			/* Print texts */
			attron(COLOR_PAIR(TITLE_CP));
			mvprintw(TITLE_MSG_Y,TITLE_MSG_X,"%s",TITLE_MSG);
			mvprintw(TITLE_MSG_Y,TITLE_LICENSE_X,"%s",TITLE_LICENSE);
			attroff(COLOR_PAIR(TITLE_CP));

			/* Refresh screen */
			refresh();

			/* Sync to FPS */
			usleep((int)(1/(double)TITLE_FPS*SECS));
		}
		stop = !start;
	}
	if(pthread_mutex_unlock(&title_m)!=0) return NULL;
}

/* Calls menu to start a new game or load a previous one */
short menu(savegame *sav) {
	ColorPair *cp;
	animation anim;
	pthread_t title_id;
	char file[FILE_MAX], entries[MENU_N][MAX_TEXT] = MENU_T;
	FILE *save;
	int i, ch, y[MENU_N] = MENU_Y;
	short choice, ret;

	if(sav==NULL) return -1;

	/* Load colorpairs */
	cp = initColorPairs();
	if(cp==NULL) return -1;

	/* Load extras */
	for(i=0;i<TITLE_EXTRAS;i++) {
		sprintf(file,"%s/extra%02d.pixmap",TITLE_DIR,i);
		anim.extras[i] = loadPixmap(file,cp);
		if(anim.extras[i]==NULL) {
			while(i>0) destroyPixmap(anim.extras[--i]);
			destroyColorPairs(cp);
			return -1;
		}
		if(movePixmap(anim.extras[i],TITLE_EXTRA_Y,TITLE_EXTRA_X)==ERR) {
			while(i>0) destroyPixmap(anim.extras[--i]);
			destroyColorPairs(cp);
			return -1;
		}
	}

	/* Load frames */
	for(i=0;i<TITLE_AFRAMES;i++) {
		sprintf(file,"%s/%02d.pixmap",TITLE_DIR,i);
		anim.frames[i] = loadPixmap(file,cp);
		if(anim.frames[i]==NULL) {
			while(i>0) destroyPixmap(anim.frames[--i]);
			for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
			destroyColorPairs(cp);
			return -1;
		}
	}

	/* Add colorpairs to title argspack */
	anim.cp = cp;

	/* Start title thread */
	start = true;
	if(pthread_create(&title_id,NULL,title,(void*)&anim)!=0) {
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}
	if(pthread_detach(title_id)!=0) {
		pthread_cancel(title_id);
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}

	/* Wait for keypress */
	getch();

	/* Stop title thread */
	if(pthread_mutex_lock(&start_m)!=0) {
		pthread_cancel(title_id);
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}
	start = false;
	if(pthread_mutex_unlock(&start_m)!=0) {
		pthread_cancel(title_id);
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}
	if(pthread_mutex_lock(&title_m)!=0) {
		pthread_cancel(title_id);
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}
	if(pthread_mutex_unlock(&title_m)!=0) {
		pthread_cancel(title_id);
		for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
		for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);
		destroyColorPairs(cp);
		return -1;
	}
	usleep(TITLE_TIMEOUT);

	/* Free used resources */
	for(i=0;i<TITLE_AFRAMES;i++) destroyPixmap(anim.frames[i]);
	for(i=0;i<TITLE_EXTRAS;i++) destroyPixmap(anim.extras[i]);

	/* Clear message */
	attron(COLOR_PAIR(TITLE_CP));
	mvprintw(TITLE_MSG_Y,TITLE_MSG_X,"%s",MENU_MSG);
	attroff(COLOR_PAIR(TITLE_CP));

	/* Loop for menu choice */
	for(choice=0,ch=0;ch!=10;ch=getch()) {
		/* Move choice */
		if(ch==KEY_UP) choice--;
		else if(ch==KEY_DOWN) choice++;
		choice = (choice<0? choice+MENU_N : choice%MENU_N);

		/* Print entries */
		for(i=0;i<MENU_N;i++) {
			attron(COLOR_PAIR(MENU_CP));
			mvprintw(y[i],MENU_X,"%s",entries[i]);
			mvprintw(y[i],MENU_X-2,i==choice?UTILS_ARROW:" ");
			attroff(COLOR_PAIR(MENU_CP));
		}
		refresh();
	}

	switch(choice) {
		case 0: /* New game */
			ret = 0;
			break;
		case 1: /* Load game */
			attron(COLOR_PAIR(MENU_CP));
			mvprintw(y[0],MENU_SAVE_X,"%s",SAVE_ASK);

			/* Retrieve slot */
			ch = getch();

			/* Valid slot */
			if(ch>'0' && ch<='0'+SAVE_SLOTS) {
				/* Open savefile */
				sprintf(file,SAVE_FORMAT,ch-'0');
				save = fopen(file,"r");
				if(save!=NULL) {
					/* Read structure binarywise */
					fread((void*)&sav->unlocked_weapons,sizeof(short),1,save);
					fread((void*)&sav->unlocked_skills,sizeof(short),1,save);
					fread((void*)&sav->max_health,sizeof(short),1,save);
					fread((void*)&sav->progress,sizeof(short),1,save);

					/* Done */
					fclose(save);
				}
				else ch = 0;
			}

			/* Garrido mode */
			if(ch==GM_KEY) {
				mvprintw(y[0]+1,MENU_SAVE_X,GM_MSG);
				sav->unlocked_weapons = GM_UW;
				sav->unlocked_skills = GM_US;
				sav->max_health = GM_MH;
				sav->progress = GM_PR;
				ret = sav->progress;
			}
			/* Print results and set return */
			else if(ch<'1' || ch>'0'+SAVE_SLOTS) { /* Error */
				mvprintw(y[0]+1,MENU_SAVE_X,"ERROR. Now quitting...");
				ret = -1;
			}
			else { /* OK */
				mvprintw(y[0]+1,MENU_SAVE_X,"%d \u2713", ch-'0');
				ret = sav->progress;
			}
			attroff(COLOR_PAIR(MENU_CP));

			getch();

			break;
		case 2: /* Tutorial */
			ret = -2;
			break;
		default: /* Quit */
			ret = -1;
			break;
	}

	destroyColorPairs(cp);

	return ret;

}

/* Prints map and returns chosen level/stage */
short map(savegame *sav) {
	FILE *map, *save;
	ColorPair *cp;
	char buf[MAP_H][MAP_BUF], file[FILE_MAX];
	int y, x, i, ch;
	short choice, temp;

	char lvl[LVL_N][MAX_TEXT] = LVL_T;
	int lvl_x[LVL_N] = LVL_X;

	char bss[BSS_N][MAX_TEXT] = BSS_T;
	int bss_x[BSS_N] = BSS_X;

	/* Calculate position */
	y = (SCREEN_H-MAP_H)/2;
	x = (SCREEN_W-MAP_W)/2;

	/* Open file */
	map = fopen(MAP_FILE,"r");
	if(map==NULL) return -1;

	/* Retrieve map */
	for(i=0;i<MAP_H&&fgets(buf[i],MAP_BUF,map)!=NULL;i++);

	/* Done with file */
	fclose(map);

	/* Load colorpairs */
	cp = initColorPairs();
	if(cp==NULL) return -1;

	/* Clear window */
	clear();

	/* Loop and wait for enter */
	for(choice=0,ch=0;ch!=10;ch=getch()) {
		/* Backup choice in case of locked level */
		temp = choice;

		/* Move to level at user input */
		switch(ch) {
			case KEY_UP:
			case KEY_DOWN:
				choice += ((choice%2)? -1:1);
				break;
			case KEY_RIGHT:
				if(choice==LVL_N+BSS_N-1)
					choice = 0;
				else if(choice==LVL_N+BSS_N-2)
					choice = 1;
				else
					choice += 2;
				break;
			case KEY_LEFT:
				if(choice==0)
					choice = 6;
				else if(choice==1)
					choice = 7;
				else
					choice -= 2;
				break;
			case 's': /* Save */
				/* Print message */
				attron(COLOR_PAIR(MAP_SELECT_CP));
				mvprintw(y+PROGRESS_Y,x+PROGRESS_X+10,"%s",SAVE_ASK);

				/* Retrieve slot */
				i = getch();

				/* Valid slot */
				if(i>'0' && i<='0'+SAVE_SLOTS) {
					/* Open savefile */
					sprintf(file,SAVE_FORMAT,i-'0');
					save = fopen(file,"w");
					if(save==NULL) {
						i = 0;
						break;
					}

					/* Write structure binarywise */
                                        fwrite((void*)&sav->unlocked_weapons,sizeof(short),1,save);
                                        fwrite((void*)&sav->unlocked_skills,sizeof(short),1,save);
                                        fwrite((void*)&sav->max_health,sizeof(short),1,save);
                                        fwrite((void*)&sav->progress,sizeof(short),1,save);

					/* Done */
					fclose(save);
				}

				/* Print results */
				if(i<'1' || i>'0'+SAVE_SLOTS) /* Error */
					mvprintw(y+PROGRESS_Y,x+PROGRESS_X+strlen(SAVE_ASK)+10,"ERROR. Try again.");
				else /* OK */
					mvprintw(y+PROGRESS_Y,x+PROGRESS_X+strlen(SAVE_ASK)+10,"%d \u2713", i-'0');

				attroff(COLOR_PAIR(MAP_SELECT_CP));

				getch();

				break;
			case 'q': /* Quit */
				destroyColorPairs(cp);
				return -1;
		}

		/* If level is locked, recover previous choice */
		if(choice>sav->progress) choice = temp;

		/* Print map */
		attron(COLOR_PAIR(MAP_CP));
		for(i=0;i<MAP_H;i++) mvprintw(y+i,x,"%s",buf[i]);

		/* Print progress */
		mvprintw(y+PROGRESS_Y,x+PROGRESS_X,"%.1f%%                    ",\
				100*(float)sav->progress/(LVL_N+BSS_N));

		/* Print levels and bosses */
		for(i=0;i<=sav->progress && i<LVL_N+BSS_N;i++) {
			if(choice==i) attron(COLOR_PAIR(MAP_SELECT_CP));
			(i%2)? mvprintw(y+BSS_Y,x+bss_x[i/2],"%s",bss[i/2]) : mvprintw(y+LVL_Y,x+lvl_x[i/2],"%s",lvl[i/2]);
			if(choice==i) attron(COLOR_PAIR(MAP_CP));
		}

		/* And update window */
		attroff(COLOR_PAIR(MAP_CP));
		refresh();
	}

	/* Unload colorpair access */
	destroyColorPairs(cp);

	/* Return final choice */
	return choice;
}

void main(int argc, char *argv[]) {
	char *locale;
	Platform *pf;
	bool stop, success;
	short menu_ret, map_ret, pf_ret;
	savegame sav;

	/* Load system locale for UTF-8 compatibility */
	locale=setlocale(LC_ALL,"");

	/* Initialize ncurses */
	initscr();
	start_color();
	keypad(stdscr,true);
	noecho();
	cbreak();
	curs_set(0);

	/* Loop game */
	stop = false;
	do {
		menu_ret = menu(&sav);
		switch(menu_ret) {
			case -1: /* Quit */
				stop = true;
				break;
			case -2: /* Tutorial */
				pf = startPlatform(0,SAVE_UW_0);
				if(pf==NULL) {
					stop = true;
					break;
				}
				pf_ret = waitForPlatform(pf);
				destroyPlatform(pf);
				stop = true;
				break;
			case 0: /* New game */
				sav.unlocked_weapons = SAVE_UW_0;
				sav.unlocked_skills = SAVE_US_0;
				sav.max_health = SAVE_MH_0;
				sav.progress = 0;
			default:
				do {
					map_ret = map(&sav);
					if(map_ret==-1) { /* Quit */
					       stop = true;
				       	       break;
					}
					if(map_ret%2) { /* Combat */
						success = startCombat(map_ret/2,sav.unlocked_skills,sav.max_health);

						/* False positive */
						if(map_ret<sav.progress) success = false;
					}
					else { /* Platform */
						do {
							/* Initialisation */
							pf = startPlatform(map_ret/2+1,sav.unlocked_weapons);
							if(pf==NULL) break;

							/* Wait for platform to finish its stuff */
							pf_ret = waitForPlatform(pf);

							/* Free resources and give escape value */
							destroyPlatform(pf);
						} while(pf_ret==1);
						success = (!pf_ret? true:false);

						/* False positive */
						if(map_ret<sav.progress) success = false;
					}

					/* Upgrade save in case of success */
					if(success) {
						switch(sav.progress++) {
							case 0: /* Level 1 */
								sav.unlocked_weapons = SAVE_UW_1;
								break;
							case 2: /* Level 2 */
								sav.unlocked_weapons = SAVE_UW_2;
								break;
							case 4: /* Level 3 */
								sav.unlocked_weapons = SAVE_UW_3;
								break;
							case 1: /* Stage 1 */
								sav.unlocked_skills = SAVE_US_1;
								sav.max_health = SAVE_MH_1;
								break;
							case 3: /* Stage 2 */
								sav.unlocked_skills = SAVE_US_2;
								sav.max_health = SAVE_MH_2;
								break;
							case 5: /* Stage 3 */
								sav.unlocked_skills = SAVE_US_3;
								sav.max_health = SAVE_MH_3;
								break;
							case 7: /* Stage 4 */
								sav.unlocked_skills = SAVE_US_4;
								sav.max_health = SAVE_MH_4;
								break;
						}
					}
				} while(map_ret!=-1);
				break;
		}
	} while(!stop);

	endwin();
}
