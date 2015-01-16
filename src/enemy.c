#include "enemy.h"

/********************/
/* PUBLIC FUNCTIONS */
/********************/

/* Tells whether a enemy is killed by a bullet, 2 means bullet remains */
short isEnemyKilledBy(enemy_t enemy, element bullet) {
	switch(enemy) {
		case ZITHERS:
		case FAIRY:
		case ZOMBIE:
			switch(bullet) {
				case BASIC: return 1;
				case WATER: return 1;
				case FIRE: return 1;
				case LIGHT: return 2;
			}
		case DRAGON1:
		case DRAGON2:
		case DRAGON3:
		case DRAGON4:
		case DRAGON5:
		case DRAGON6:
		case DRAGON7:
		case DRAGON8:
		case DRAGON9:
		case FLAME:
			switch(bullet) {
				case WATER: return 1;
				case LIGHT: return 2;
			}
         	case CHARLOTTE:
	        case BAT:
		        switch(bullet) {
			        case FIRE: return 1;
				case LIGHT: return 2;
			}
		case CROW1:
		case CROW2:
			switch(bullet) {
			        case FIRE: return 1;
				case WATER: return 1;
				case LIGHT: return 2;
			}
        	case GHOST:
		case CERBERUS:
			switch(bullet) {
				case LIGHT: return 2;
			}
		default: return 0;
	}
	return 0;
}
