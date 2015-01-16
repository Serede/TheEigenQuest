#include "combat_interface.h"

/********************/
/*    STRUCTURES    */
/********************/

struct {
  WINDOW* enemy, *player;
  ColorPair* cp;
} combat_interface;


/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

/* DEPRECATED */
/* Writes a text on the bottom window and waits for an user action to continue */
status _setTextBottom(char* text) {
  WINDOW *window;
  int i, j;
  char* final_text;
  refresh();
  
  if(text==NULL) return ERR;
  final_text=strdup(text);
  
  /* Creation of a window */
  window=newwin(BOTTOM_WIN_HEIGHT, BOTTOM_WIN_WIDTH, BOTTOM_WIN_Y, BOTTOM_WIN_X);
  if(window==NULL) return ERR;
  keypad(window, TRUE);
  wbkgd(window, COLOR_PAIR(BOTTOM_WIN_COLOR));
  
  /* Prints the text and the border*/
  mvwprintw(window, BOTTOM_WIN_TEXTY, BOTTOM_WIN_TEXTX, final_text);
  box(window, 0, 0);
  wrefresh(window);
  
  /* Waits until an user action */
  getch();
  free(final_text);
  delwin(window);
  return OK;
}

/* Function that waits until the text ends and the user push enter key */
void* callInputThread(void* vargs) {
  bool* flag = (bool*)vargs;
  char c;
  while(c=getch()) {
    if(c==10&&*flag==true) return NULL;
    *flag = true; /* If you push another key, the complete text  is printed*/
    c=1;
  } 
}

/* DEPRECATED it prints the menu without desc. It is called when desc==NULL */
/* Sets a dialog and lets the user choose an option. It just return         */
/* the selected option or ERR otherwise. Take care of the n_choices         */
/* If it isnt correct, it will end in a SEGFAULT signal.                    */ 
int _setTextDialog(int n_choices, char* title, char **options ) {
  ITEM **my_items, *cur_item;
  int c, choosing=1;				
  MENU *my_menu;
  WINDOW *my_menu_win, *desc_win;
  int  i, selected_item;
  char** choices;

  if(n_choices <=0 || title==NULL || options==NULL)
    return ERR;
  
  /* Create items */
  choices = (char**) calloc(n_choices+1, sizeof(char*));
  if(choices==NULL) return ERR;
  for (i=0;i<n_choices;i++)
    choices[i]=strdup(options[i]);
  choices[n_choices] = NULL;
  my_items = (ITEM **)calloc(n_choices+1, sizeof(ITEM *));
  if(my_items==NULL){
    free(choices);
    return ERR;
  }
  
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i], choices[i]);

  /* Create menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set menu option not to show the description */
  /* This option will duplicated the item.       */
  menu_opts_off(my_menu, O_SHOWDESC);
  refresh();

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(BOTTOM_WIN_HEIGHT, BOTTOM_WIN_WIDTH, BOTTOM_WIN_Y, BOTTOM_WIN_X);
  keypad(my_menu_win, TRUE);
     
  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, MENU_SUBWIN_HEIGHT, MENU_SUBWIN_WIDTH, MENU_SUBWIN_Y, MENU_SUBWIN_X));
  set_menu_format(my_menu, MENU_N_ROWS, MENU_N_COLUMNS);
  set_menu_mark(my_menu, MENU_POINTER);
  set_menu_spacing(my_menu, 0, MENU_SPACING_Y, MENU_SPACING_X);
  desc_win=derwin(my_menu_win, MENU_DESC_WIN_HEIGHT,MENU_DESC_WIN_WIDTH, MENU_DESC_WIN_Y, MENU_DESC_WIN_X);
  
  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  mvwprintw(my_menu_win, MENU_TITLE_TEXTY, MENU_TITLE_TEXTX, title);
  refresh();
  wbkgd(my_menu_win,COLOR_PAIR(MENU_DEFAULT_COLOR));

  /* Post the menu */
  set_menu_back(my_menu, COLOR_PAIR(MENU_DEFAULT_COLOR));
  set_menu_fore(my_menu, COLOR_PAIR(MENU_SELECTED_COLOR));
  post_menu(my_menu);
  wrefresh(my_menu_win);
  refresh();

  /* Lets choose what you want! */
  while(choosing)
    {   
      c = wgetch(my_menu_win);    
      switch(c)
	{
	case KEY_DOWN:
	    menu_driver(my_menu, REQ_DOWN_ITEM);
	    break;
	case KEY_UP:
	  menu_driver(my_menu, REQ_UP_ITEM);
	  break;
	case KEY_LEFT:
	  menu_driver(my_menu, REQ_LEFT_ITEM);
	  break;
	case KEY_RIGHT:
	  menu_driver(my_menu, REQ_RIGHT_ITEM);
	  break;
	case KEY_NPAGE:
	  menu_driver(my_menu, REQ_SCR_DPAGE);
	  break;
	case KEY_PPAGE:
	  menu_driver(my_menu, REQ_SCR_UPAGE);
	  break;
	case 10:	/* Enter */
	  choosing=0;
	  cur_item=current_item (my_menu);
	}
      wmove(my_menu_win,0, 0);
      wrefresh(my_menu_win);
    }	

  /* What item was selected */
  for(i = 0; i < n_choices; ++i)
    if(cur_item==my_items[i])
      break;
  selected_item=i;

  /* Unpost and free all the memory taken up */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i) {
    free_item(my_items[i]);
    free(choices[i]);
  }
  free(my_items);
  free(choices[n_choices]);
  free(choices);
  delwin(my_menu_win);
  return selected_item;
}

/* Return the selected item for a list of items */
/* I don't know why ncurses did not include something like this */
int getCurrentIndex(ITEM* cur, ITEM** my_items, int n_choices) {
  int i;
  for(i = 0; i < n_choices; ++i)
    if(cur==my_items[i])
      break;
  return i;

}

/* Function that changes the format according to arg1 and arg2 */
void execCommand(WINDOW* win, char arg1, char arg2) {
  int action, arg;
  if(win==NULL) return;
  switch(arg1) {
    case 'e':
      action=1;
      break;
    case 'd':
      action=2;
      break;
    default:
      action=0;
  }
  switch(arg2) {
    case 'k':
      arg=BLACK_CI;
      break;
    case 'r':
      arg=RED_CI;
      break;
    case 'g':
      arg=GREEN_CI;
      break;
    case 'y':
      arg=YELLOW_CI;
      break;
    case 'b':
      arg=BLUE_CI;
      break;
    case 'm':
      arg=MAGENTA_CI;
      break;
    case 'c':
      arg=CYAN_CI;
      break;
    case 'w':
      arg=WHITE_CI;
      break;
    default: 
      arg=0;
  }
  if(arg==0) return;
  if(action==0) return;
  if(action==1) {
    wattron(win, COLOR_PAIR(arg));
  }
  if(action==2) {
    wattroff(win, COLOR_PAIR(arg));
  }
}
/*********************/
/* PUBLIC  FUNCTIONS */
/*********************/

/* Initiates the basics structures */
status initCombatInterface() {
  ColorPair* cp;
  /* Init colorpairs */
  cp=initColorPairs();
  init_pair(WHITE_CI, COLOR_WHITE, COLOR_WHITE);
  if(cp==NULL) return ERR;
  combat_interface.cp=cp;

  /* Allocates memory for each window */
  combat_interface.enemy=newwin(ENEMY_WIN_HEIGHT, ENEMY_WIN_WIDTH, ENEMY_WIN_Y, ENEMY_WIN_X);
  combat_interface.player=newwin(PLAYER_WIN_HEIGHT, PLAYER_WIN_WIDTH, PLAYER_WIN_Y, PLAYER_WIN_X);

  /*Set up color of each one */
  wbkgd(combat_interface.player, COLOR_PAIR(PLAYER_WIN_COLOR));
  wbkgd(combat_interface.enemy, COLOR_PAIR(ENEMY_WIN_COLOR));
  refresh();

  /* Borders */
  box(combat_interface.enemy, 0,0);
  box(combat_interface.player, 0,0);
  
  wrefresh(combat_interface.player);
  wrefresh(combat_interface.enemy);
  return OK;
}

/* Frees all the memory asociated to the module */
void destroyCombatInterface(){
  delwin(combat_interface.enemy);
  delwin(combat_interface.player);
  destroyColorPairs(combat_interface.cp);
}

/* Writes the enemy name */
status setTextEnemy(char* text) {
  int len=strlen(text), x, y, i;
  if(text==NULL) return ERR;
  mvwprintw(combat_interface.enemy, ENEMY_WIN_TEXTY, ENEMY_WIN_TEXTX, "\t\t\t\t\t\t");
  //mvwprintw(combat_interface.enemy,ENEMY_WIN_TEXTY,ENEMY_WIN_TEXTX,text);
  for(i=0, x=ENEMY_WIN_TEXTX, y=ENEMY_WIN_TEXTY;i<len;i++) {
    /* \t = TAB_LENGTH spaces */
    if(text[i]=='\t') {
      x+=TAB_LENGTH; continue;
    }
    else if(text[i]==COMMAND_CHAR) {
      execCommand(combat_interface.enemy,text[i+1],text[i+2]);
      i+=2; continue; 
    }
    mvwaddch(combat_interface.enemy, y, x++, text[i]);
  }
  mvwprintw(combat_interface.enemy, y, x, "\t\t\t\t");
  wrefresh(combat_interface.enemy);
  return OK;
}

/* Writes the player name */
status setTextPlayer(char* text) {
  int len=strlen(text), x, y, i;
  if(text==NULL) return ERR;
  //mvwprintw(combat_interface.player,PLAYER_WIN_TEXTY,PLAYER_WIN_TEXTX,text);
  mvwprintw(combat_interface.player, PLAYER_WIN_TEXTY, PLAYER_WIN_TEXTX, "\t\t\t\t\t\t");
  for(i=0, x=PLAYER_WIN_TEXTX, y=PLAYER_WIN_TEXTY;i<len;i++) {
    /* \t = TAB_LENGTH spaces */
    if(text[i]=='\t') {
      x+=TAB_LENGTH; continue;
    }
    else if(text[i]==COMMAND_CHAR) {
      execCommand(combat_interface.player,text[i+1],text[i+2]);
      i+=2; continue; 
    }
    mvwaddch(combat_interface.player, y, x++, text[i]);
  }
  mvwprintw(combat_interface.player, y, x, "\t");
  wrefresh(combat_interface.player);
  return OK;
}

/* Prints a text with a delay */
status setTextBottom(char* text) {
  WINDOW *window;
  int len, i, j, x, y;
  bool flag=false;
  void* status;
  pthread_t input_thread;
  pthread_attr_t attr;
  refresh();
  
  if(text==NULL) return ERR;
  
  /* Creation of a window */
  window=newwin(BOTTOM_WIN_HEIGHT, BOTTOM_WIN_WIDTH, BOTTOM_WIN_Y, BOTTOM_WIN_X);
  if(window==NULL) return ERR;
  keypad(window, TRUE);
  wbkgd(window, COLOR_PAIR(BOTTOM_WIN_COLOR));
  len=strlen(text);
  box(window, 0,0);
  wrefresh(window);

  /* Call a joinable input thread */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&input_thread,&attr, callInputThread, (void*)&flag);

  /* Prints each char with a delay if the flag is true */
  for(i=0,x=BOTTOM_WIN_TEXTX,y=BOTTOM_WIN_TEXTY;i<strlen(text);i++) {
    /* \n = next line */
    if(text[i]=='\n') {
      y++; x=BOTTOM_WIN_TEXTX; continue;
    } 
    /* \t = TAB_LENGTH spaces */
    else if(text[i]=='\t') {
      x+=TAB_LENGTH; continue;
    }
    else if(text[i]==COMMAND_CHAR) {
      execCommand(window,text[i+1],text[i+2]);
      i+=2; continue; 
    }
    /* End of line case */
    if(x>158) {
      x=x%158; y++;
    }
    /* Prints the char */
    mvwaddch(window, y, x++, text[i]);
    wrefresh(window);
    if(flag==false)  usleep(DELAY_SET_TEXT_BOTTOM);
  }
  /* Set the flag to true so that the thread waits for an enter and then it is joinable */
  flag=true;
  pthread_join(input_thread, &status);
  return OK;
}


/* Sets a dialog and lets the user choose an option. It just return         */
/* the selected option or ERR otherwise. Take care of the n_choices         */
/* If it isnt correct, it will end in a SEGFAULT signal.                    */
int setTextDialog(int n_choices, char* title, char **options, char **desc ) {
  ITEM **my_items, *cur_item;
  int c, choosing=1;				
  MENU *my_menu;
  WINDOW *my_menu_win,*desc_win,*sub_menu;
  int  i, selected_item, curr;
  char** choices, *j;
  if(desc==NULL) return _setTextDialog(n_choices, title, options);

  if(n_choices <=0 || title==NULL || options==NULL)
    return ERR;
  
  /* Create items */
  choices = (char**) calloc(n_choices+1, sizeof(char*));
  if(choices==NULL) return ERR;
  for (i=0;i<n_choices;i++) choices[i]=strdup(options[i]);
  choices[n_choices] = NULL;
  my_items = (ITEM **)calloc(n_choices+1, sizeof(ITEM *));
  if(my_items==NULL){
    free(choices);
    return ERR;
  }
  
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i], choices[i]);
  my_items[n_choices]=NULL;
  /* Create menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set menu option not to show the description */
  /* This option will duplicated the item.       */
  menu_opts_off(my_menu, O_SHOWDESC);
  refresh();

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(BOTTOM_WIN_HEIGHT, BOTTOM_WIN_WIDTH, BOTTOM_WIN_Y, BOTTOM_WIN_X);
  keypad(my_menu_win, TRUE);
  
  desc_win=derwin(my_menu_win, MENU_DESC_WIN_HEIGHT,MENU_DESC_WIN_WIDTH, MENU_DESC_WIN_Y, MENU_DESC_WIN_X);
  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu,sub_menu = derwin(my_menu_win, MENU_SUBWIN_HEIGHT, MENU_SUBWIN_WIDTH, MENU_SUBWIN_Y, MENU_SUBWIN_X));
  set_menu_format(my_menu, MENU_N_ROWS, MENU_N_COLUMNS);
  set_menu_mark(my_menu, MENU_POINTER);
  set_menu_spacing(my_menu, 0, MENU_SPACING_Y, MENU_SPACING_X);

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  mvwprintw(my_menu_win, MENU_TITLE_TEXTY, MENU_TITLE_TEXTX, title);
  refresh();
  wbkgd(my_menu_win,COLOR_PAIR(MENU_DEFAULT_COLOR));

  /* Post the menu */
  set_menu_back(my_menu, COLOR_PAIR(MENU_DEFAULT_COLOR));
  set_menu_fore(my_menu, COLOR_PAIR(MENU_SELECTED_COLOR));
  post_menu(my_menu);
  wrefresh(my_menu_win);
  refresh();
  wbkgd(desc_win,COLOR_PAIR(MENU_DESC_WIN_COLOR));
  curr = 0;
  mvwprintw(desc_win,  0,0, desc[curr]);
  wrefresh(desc_win);
  wrefresh(my_menu_win);
  
  /* Lets choose what you want! */
  while(choosing)
    {   
      c = wgetch(my_menu_win);    
      switch(c)
	{
	case KEY_DOWN:
	    menu_driver(my_menu, REQ_DOWN_ITEM);
	    break;
	case KEY_UP:
	  menu_driver(my_menu, REQ_UP_ITEM);
	  break;
	case KEY_LEFT:
	  menu_driver(my_menu, REQ_LEFT_ITEM);
	  break;
	case KEY_RIGHT:
	  menu_driver(my_menu, REQ_RIGHT_ITEM);
	  break;
	case KEY_NPAGE:
	  menu_driver(my_menu, REQ_SCR_DPAGE);
	  break;
	case KEY_PPAGE:
	  menu_driver(my_menu, REQ_SCR_UPAGE);
	  break;
	case 10:	/* Enter */
	  choosing=0;
	  cur_item=current_item (my_menu);
	}
      mvwprintw(desc_win, 0,0 ,SPACES_STRING);
      curr = getCurrentIndex(current_item(my_menu),my_items, n_choices);
      mvwprintw(desc_win, 0,0 ,desc[curr]);
      wmove(my_menu_win,0, 0);
      wrefresh(desc_win);
      wrefresh(my_menu_win);
    }	

  /* What item was selected */
  cur_item=current_item (my_menu);
  for(i = 0; i < n_choices; ++i)
    if(cur_item==my_items[i])
      break;
  selected_item=i;

  /* Unpost and free all the memory taken up */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i) {
    free_item(my_items[i]);
    free(choices[i]);
  }
  free(my_items);
  free(choices[n_choices]);
  free(choices);
  delwin(desc_win);
  delwin(sub_menu);
  delwin(my_menu_win);
  return selected_item;
}

/* Set enemy health bar to value/max hearts */
status setEnemyBar(int value, int max) {
  char bar[STRING_MAX_LENGTH]="\0";
  int i=0;
  if(value<0 || value>max) return ERR;
  /* Conversion to interface ratio */
  if (value!=0) {
    value*=RATIO_CONV_ENEMY_BAR;
    value==0?1:value;
  }  
  else --value;
  max*=RATIO_CONV_ENEMY_BAR;
  /* Full hearts */
  for(i=0; i<=value;i+=1) 
    strcat(bar,FULL_HEART);
  /* Programmers hearts */
  for(;i<=max;i+=1)
    strcat(bar, EMPTY_HEART);
  wattron(combat_interface.enemy, COLOR_PAIR(HEART_COLOR));
  mvwprintw(combat_interface.enemy, 2, 1, bar);
  wattroff(combat_interface.enemy, COLOR_PAIR(HEART_COLOR));
  wrefresh(combat_interface.enemy);
  return OK;
}

/* Set player health bar to value/max hearts */
status setPlayerBar(int value, int max) {
  char bar[STRING_MAX_LENGTH]="\0";
  int i;
  if(value<0 || value>max) return ERR;
  /* Conversion  to interface ratio */
  if(value!=0) {
    value*=RATIO_CONV_PLAYER_BAR;
    value==0?1:value;
  }
  else --value;
  max*=RATIO_CONV_PLAYER_BAR;
  /* Full hearts */
  for(i=0; i<=value;i+=1) 
    strcat(bar,FULL_HEART);
  /* Programmers hearts */
  for(;i<=max;i+=1)
    strcat(bar, EMPTY_HEART);
  wattron(combat_interface.player, COLOR_PAIR(HEART_COLOR));
  mvwprintw(combat_interface.player, 2, 1,bar);
  wattroff(combat_interface.player, COLOR_PAIR(HEART_COLOR));
  wrefresh(combat_interface.player);
  return OK;
}

/* Prints the player pixmap */
status setPlayerPixmap(char* path){
  WINDOW* win;
  Pixmap* pm;

  /* Loads a pixmap */
  pm=loadPixmap(path, combat_interface.cp);
  if(pm==NULL) return ERR;

  /* Creates a window, just for safety, the pixmap 
     could be bigger than expected */
  win = newwin(PLAYER_PM_HEIGHT, PLAYER_PM_WIDTH, PLAYER_PM_Y,PLAYER_PM_X );

  /* Background to the window, if the pixmap is not
     in the correct size, it we will see this color */
  wbkgd(win, COLOR_PAIR(DEFAULT_BACKGROUND_COLOR));
  /*  movePixmap(pm, PLAYER_PM_INNER_Y, PLAYER_PM_INNER_X);*/
  /* Prints it */
  if(printPixmap(win, pm, false)) return ERR;
  wrefresh(win);
  destroyPixmap(pm);
  delwin(win);
  return OK;
}

/* Prints the enemy pixmap */
status setEnemyPixmap(char* path){
  Pixmap* pm;
  WINDOW* win;

  /* Loads a pixmap */
  pm=loadPixmap(path, combat_interface.cp);
  if(pm==NULL) return ERR;

  /* Creates a window, just for safety, the pixmap 
     could be bigger than expected */
  win = newwin(ENEMY_PM_HEIGHT, ENEMY_PM_WIDTH, ENEMY_PM_Y,ENEMY_PM_X );

  /* Background to the window, if the pixmap is not
     in the correct size, it we will see this color */
  wbkgd(win, COLOR_PAIR(DEFAULT_BACKGROUND_COLOR));
  /*movePixmap(pm, ENEMY_PM_INNER_Y, ENEMY_PM_INNER_X);*/
  if(printPixmap(win, pm, false)) return ERR;
  refresh();
  /* Prints it */
  wrefresh(win);
  destroyPixmap(pm);
  delwin(win);
  return OK;
}
