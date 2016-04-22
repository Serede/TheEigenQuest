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

#ifdef COMPILING_ON_OSX
// Code taken from:  http://stackoverflow.com/a/9781275/3080396

#include <sys/time.h>
#define CLOCK_REALTIME (0)

static int clock_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

#endif
