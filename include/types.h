#ifndef _TYPES_H
#define _TYPES_H

#include <ctype.h>
#include <locale.h>
#include <menu.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define FILE_MAX 1024
#define EOS '\0'
#define SECS 1000000
#define ELEMENTS_N 4

typedef short status;
typedef enum {UP=0, DOWN, RIGHT, LEFT} side;
typedef enum {BASIC=0, WATER, FIRE, LIGHT} element;
typedef enum {TUTO=0, LVL1, LVL2, LVL3, LVL4} level;
typedef enum {BILIAR=0, CYCLOPS, IRONMAN, CUERVS} stage;

#endif
