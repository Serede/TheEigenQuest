#ifndef _UTILS_H
#define _UTILS_H

#include "color.h"
#include "types.h"

/* Frame character macros */
#define UTILS_UL "\u256D"
#define UTILS_UR "\u256E"
#define UTILS_DL "\u2570"
#define UTILS_DR "\u256F"
#define UTILS_ML "\u251C"
#define UTILS_MR "\u2524"
#define UTILS_LH "\u2500"
#define UTILS_LV "\u2502"
#define UTILS_ARROW "\u27A4"
#define UTILS_SEPARATOR "/"
#define UTILS_CONT "Continue"
#define UTILS_QUIT "Quit"

/* Prints fancy text in given position using color and/or in bold */ 
void showText(WINDOW *win, int y, int x, char *text, short cp, bool bold);

/* Prints title and body as a height-width-centered dialog in window 
 * style: 0 = {Continue}, 1 = {Continue,Quit}
 * cp: 0 = Default, n>0 = Custom */
short showDialog(WINDOW *win, int height, int width, char *title, char *body, short style, short cp);

#endif
