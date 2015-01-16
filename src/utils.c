#include "utils.h"

/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Prints fancy text in given position using color and/or in bold */ 
void showText(WINDOW *win, int y, int x, char *text, short cp, bool bold) {
	int i, text_len;
	if(win==NULL||text==NULL||cp<0||cp>NCOLORPAIRS) return;

	/* Get text length */
	text_len = strlen(text);

	/* Print frame... */
	if(bold) wattron(win,A_BOLD);
	if(cp) wattron(win,COLOR_PAIR(cp));

	/* ...first row */
	mvwprintw(win,y,x,UTILS_UL); /* Up left corner */
	for(i=1;i<text_len+1;i++) /* Up border */
		mvwprintw(win,y,x+i,UTILS_LH);
	mvwprintw(win,y,x+i,UTILS_UR); /* Up right corner */

	/* ...title row */
	mvwprintw(win,y+1,x,UTILS_LV); /* Left border */
	for(i=1;i<text_len+1;i++) /* Fill with spaces */
		mvwprintw(win,y+1,x+i," ");
	mvwprintw(win,y+1,x+i,UTILS_LV); /* Right border */
	/* Print actual text */
	mvwprintw(win,y+1,x+1,"%s",text);

	/* ...last row */
	mvwprintw(win,y+2,x,UTILS_DL); /* Down left corner */
	for(i=1;i<text_len+1;i++) /* Down border */
		mvwprintw(win,y+2,x+i,UTILS_LH);
	mvwprintw(win,y+2,x+i,UTILS_DR); /* Down right corner */

	/* Restore attributes */
	if(cp) wattroff(win,COLOR_PAIR(cp));
	if(bold) wattroff(win,A_BOLD);
}

/* Prints title and body as a height-width-centered dialog in window 
 * style: 0 = {Continue}, 1 = {Continue,Quit}
 * cp: 0 = Default, n>0 = Custom */
short showDialog(WINDOW *win, int height, int width, char *title, char *body, short style, short cp) {
	char *string, *tofree;
	char **chunks=NULL, **more_chunks=NULL;
	int i, j, chunks_n, chunks_len, y, x;
	if(win==NULL||height<0||width<0) return -1;
	if(title==NULL||body==NULL) return -1;
	if(style<0||style>1||cp<0||cp>NCOLORPAIRS) return -1;

	/* Get chunks */
	string = strdup(body);
	if(string==NULL) return -1;
	tofree = string;
	i = 0;
	do {
		more_chunks = (char**)realloc(chunks,++i*sizeof(char*));
		if(more_chunks==NULL) {
			free(chunks);
			free(tofree);
			return -1;
		}
		chunks = more_chunks;
		chunks[i-1] = strsep(&string,UTILS_SEPARATOR);
	} while(chunks[i-1]!=NULL);
	chunks_n = i-1;
	chunks_len = strlen(chunks[0]);
	
	/* Calculate position */
	y = (height-(chunks_n+6))/2;
	x = (width-(chunks_len+4))/2;

	/* Print frame... */

	/* ...first row */
	mvwprintw(win,y,x,UTILS_UL); /* Up left corner */
	for(i=1;i<3+chunks_len;i++) /* Up border */
		mvwprintw(win,y,x+i,UTILS_LH);
	mvwprintw(win,y,x+i,UTILS_UR); /* Up right corner */

	/* ...title row */
	mvwprintw(win,y+1,x,UTILS_LV); /* Left border */
	for(i=1;i<3+chunks_len;i++) /* Fill with spaces */
		mvwprintw(win,y+1,x+i," ");
	mvwprintw(win,y+1,x+i,UTILS_LV); /* Right border */
	/* Print actual text */
	if(cp) wattron(win,COLOR_PAIR(cp));
	mvwprintw(win,y+1,x+((chunks_len+4)-strlen(title))/2,"%s",title);
	if(cp) wattroff(win,COLOR_PAIR(cp));

	/* ...third row */
	mvwprintw(win,y+2,x,UTILS_ML); /* Mid left corner */
	for(i=1;i<3+chunks_len;i++) /* Mid border */
		mvwprintw(win,y+2,x+i,UTILS_LH);
	mvwprintw(win,y+2,x+i,UTILS_MR); /* Mid right corner */

	/* ...body rows */
	for(j=0;j<chunks_n;j++) {
		mvwprintw(win,y+3+j,x,UTILS_LV); /* Left border */
		for(i=1;i<3+chunks_len;i++) /* Fill with spaces */
			mvwprintw(win,y+3+j,x+i," ");
		mvwprintw(win,y+3+j,x+i,UTILS_LV); /* Right border */
		/* Print actual text */
		if(cp) wattron(win,COLOR_PAIR(cp));
		mvwprintw(win,y+3+j,x+2,"%s",chunks[j]);
		if(cp) wattroff(win,COLOR_PAIR(cp));
	}

	/* ...last three rows */
	mvwprintw(win,y+chunks_n+3,x,UTILS_ML); /* Mid left corner */
	for(i=1;i<3+chunks_len;i++) /* Mid border */
		mvwprintw(win,y+chunks_n+3,x+i,UTILS_LH);
	mvwprintw(win,y+chunks_n+3,x+i,UTILS_MR); /* Mid right corner */
	mvwprintw(win,y+chunks_n+4,x,UTILS_LV); /* Left border */
	for(i=1;i<3+chunks_len;i++) /* Fill with spaces */
		mvwprintw(win,y+chunks_n+4,x+i," ");
	mvwprintw(win,y+chunks_n+4,x+i,UTILS_LV); /* Right border */
	mvwprintw(win,y+chunks_n+5,x,UTILS_DL); /* Down left corner */
	for(i=1;i<3+chunks_len;i++) /* Down border */
		mvwprintw(win,y+chunks_n+5,x+i,UTILS_LH);
	mvwprintw(win,y+chunks_n+5,x+i,UTILS_DR); /* Down right corner */

	/* Free temporal strings */
	free(chunks);
	free(tofree);

	/* Loop choice */
	switch(style) {
		case 0:
			mvwprintw(win,y+chunks_n+4,x+((chunks_len+4)-strlen(UTILS_CONT))/2,"%s %s",UTILS_ARROW,UTILS_CONT);
			fflush(stdin);
			for(i=0;i!=10;i=getch()); /* Wait for enter */
			return 0;
		case 1:
			fflush(stdin);
			for(i=0,j=0;i!=10;i=getch()) { /* Wait for enter */
				if(i==KEY_LEFT || i==KEY_RIGHT || i==KEY_UP || i==KEY_DOWN) j = !j; /* Switch option */
				mvwprintw(win,y+chunks_n+4,x+((chunks_len+4)-(strlen(UTILS_CONT)+strlen(UTILS_QUIT)+3))/2,"%s   %s",UTILS_CONT,UTILS_QUIT);
				mvwprintw(win,y+chunks_n+4,x+((chunks_len+4)-(strlen(UTILS_CONT)+strlen(UTILS_QUIT)+3))/2-2,"%s",j?" ":UTILS_ARROW);
				mvwprintw(win,y+chunks_n+4,x+((chunks_len+4)-(strlen(UTILS_CONT)+strlen(UTILS_QUIT)+3))/2+strlen(UTILS_CONT)+1,"%s",j?UTILS_ARROW:" ");
				refresh();
			}
			return j;
	}
}
