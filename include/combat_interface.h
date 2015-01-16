#ifndef _COMBAT_INTERFACE
#define _COMBAT_INTERFACE

#include "types.h"
#include "color.h"
#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <stdio.h>

/* Default values that only are shown 
   if crazy things happens */
#define DEFAULT_BACKGROUND_COLOR 9
#define STRING_MAX_LENGTH 1000



/* Bottom window size */
#define BOTTOM_WIN_WIDTH 160
#define BOTTOM_WIN_HEIGHT 10
#define BOTTOM_WIN_X 0
#define BOTTOM_WIN_Y 30
#define BOTTOM_WIN_COLOR 49
#define BOTTOM_WIN_TEXTX 1
#define BOTTOM_WIN_TEXTY 2
#define TAB_LENGTH 4
#define DELAY_SET_TEXT_BOTTOM 50000

/* Menu settings */
#define MENU_POINTER "> "
#define MENU_SUBWIN_WIDTH 92
#define MENU_SUBWIN_HEIGHT 6
#define MENU_SUBWIN_X 1
#define MENU_SUBWIN_Y 3
#define MENU_N_ROWS 3
#define MENU_N_COLUMNS 3
#define MENU_SPACING_X 8
#define MENU_SPACING_Y 2
#define MENU_SELECTED_COLOR 50
#define MENU_DEFAULT_COLOR 49
#define MENU_TITLE_TEXTX 3
#define MENU_TITLE_TEXTY 2
#define MENU_DESC_WIN_WIDTH 62 
#define MENU_DESC_WIN_HEIGHT 7
#define MENU_DESC_WIN_X 96
#define MENU_DESC_WIN_Y 2
#define MENU_DESC_WIN_COLOR 49

/* Enemy window size */
#define ENEMY_WIN_WIDTH 160
#define ENEMY_WIN_HEIGHT 4
#define ENEMY_WIN_X 0
#define ENEMY_WIN_Y 0
#define ENEMY_WIN_COLOR 49
#define ENEMY_WIN_TEXTX 1
#define ENEMY_WIN_TEXTY 1

/* Player window size */
#define PLAYER_WIN_WIDTH 60
#define PLAYER_WIN_HEIGHT 4
#define PLAYER_WIN_X 100
#define PLAYER_WIN_Y 26
#define PLAYER_WIN_COLOR 49
#define PLAYER_WIN_TEXTX 1
#define PLAYER_WIN_TEXTY 1


/* Bar max lengths */
#define MAX_BAR_ENEMY 158
#define MAX_BAR_PLAYER 58
#define RATIO_CONV_ENEMY_BAR 0.175
#define RATIO_CONV_PLAYER_BAR 0.14

/* Unicode strings */
#define EMPTY_HEART "\u2661"
#define FULL_HEART "\u2665"
#define HEART_COLOR 25
#define SPACES_STRING	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n"\
			"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n"\
			"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n"\
			"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n"\
			"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\n"

/* Player Pixmap size*/
#define PLAYER_PM_WIDTH 60 
#define PLAYER_PM_HEIGHT 22
#define PLAYER_PM_X 100
#define PLAYER_PM_Y 4
#define PLAYER_PM_INNER_X 0
#define PLAYER_PM_INNER_Y 0

/* Enemy Pixmap size*/
#define ENEMY_PM_WIDTH 100 
#define ENEMY_PM_HEIGHT 26
#define ENEMY_PM_X 0
#define ENEMY_PM_Y 4
#define ENEMY_PM_INNER_X 0
#define ENEMY_PM_INNER_Y -2

/* Colors */
#define BLACK_CI 13
#define RED_CI 25
#define GREEN_CI 35
#define YELLOW_CI 43
#define BLUE_CI 49
#define MAGENTA_CI 53
#define CYAN_CI 55
#define WHITE_CI 57

#define COMMAND_CHAR '$'

/* Initiates the basics structures */
status initCombatInterface();

/* Frees all the memory asociated to the module */
void destroyCombatInterface();

/* Writes a text on the bottom window and waits for an user action to continue */
status setTextBottom(char* text);

/* Sets a dialog and let the user choose an option. It just returns */
/* the selected option or ERR otherwise. Take care of  n_choices    */
/* If it isnt correct, it will end in a SEGFAULT signal             */
int setTextDialog(int n_choices, char* title, char **options, char** desc );

/* Writes the player name */
status setTextPlayer(char* text);

/* Writes the enemy name */
status setTextEnemy(char* text);

/* Set enemy health bar to value/max hearts */
status setEnemyBar(int value, int max);

/* Set player health bar to value/max hearts */
status setPlayerBar(int value, int max);

/* Prints the player pixmap */
status setPlayerPixmap(char* path);

/* Prints the enemy pixmap */
status setEnemyPixmap(char* path);

#endif
