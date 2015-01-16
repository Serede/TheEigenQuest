#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#define BATTLE_START_MSG "\tPrepare for battle! $er%s$dr is come.\n"
#define DO_YOU_LOVE_FANSI_C "Whatcha wanna do?"

#define ERR_MSG "\tSomething went wrong. Go ask the programmers e.e\n"

#define WIN_MSG "\t$egYou have defeated %s!$dg\n\tCongratulations, you can now advance to the next level.\n"
#define LOSE_MSG "\t$erYou have been defeated$dr.\n\tYou now lie on the ground, your corpse ready for the picking...\n"

#define ACTION_ATTEMPT_MSG "\t%s used $ec%s$dc!"

#define NO_PPOINTS_MSG "\tYou have no points left for that skill!\n"

#define PARALIZED_FAIL_MSG "\t%s is $eyparalized$dy and cannot attack!"
#define ATT_FAILED_MSG "\tThe attack failed!\n"

#define YOU_SUFFERED_DAMAGE_MSG "\tYou suffered $er%d$dr points of damage!\n"
#define BOSS_SUFFERED_DAMAGE_MSG "\t%s suffered $er%d$dr points of damage!\n"
#define YOU_HEALED_MSG "\tYou recovered $eg%d$dg health points!\n"
#define BOSS_HEALED_MSG "\t%s healed himself, damn him!\n"

#define ATT_UP_MSG "\t%s's attack went up!"
#define DEF_UP_MSG "\t%s's defense went up!\n"
#define ATT_DOWN_MSG "\t%s's attack went down!\n"
#define DEF_DOWN_MSG "\t%s's defense went down!\n"
#define BLIND_MSG "\t%s got $ekblind$dk!\n"
#define POISON_MSG "\t%s got $egpoisoned$dg!\n"
#define PARALIZED_MSG "\t%s got $eyparalized$dy!\n"
#define MINDFUCK_MSG "\t%s got $emmindfucked$dm!\n"

#define YOU_CURED_MSG "\tYou $egcured$dg your ailments.\n"
#define YOU_SUFFERED_POISON_DAMAGE_MSG "\tOuch! The $egpoison$dg effect drains your health!\n"

#define YOU_CURED_FROM_POISON_MSG "\tYay! You are no longer $egpoisoned$dg!"
#define YOU_CURED_FROM_BLIND_MSG "\tOh! You are no longer $ekblind$dk!"
#define YOU_CURED_FROM_MINDFUCKED_MSG "\tPhew! You are no longer $emmindfucked$dm!"
#define YOU_CURED_FROM_PARALIZED_MSG "\tYeah! You are no longer $eyparalized$dy!"

#define BOSS_CURED_MSG "\t%s cured himself, damn him!\n"
#define BOSS_SUFFERED_POISON_DAMAGE_MSG "\tHa-ha! The $egpoison$dg effect drains the enemy's health!\n"

#define BOSS_CURED_FROM_POISON_MSG "\tDamn!%s is no longer $egpoisoned$dg."
#define BOSS_CURED_FROM_BLIND_MSG "\tWatch out! %s is no longer $ekblind$dk."
#define BOSS_CURED_FROM_MINDFUCKED_MSG "\tDoh! %s is no longer $emmindfucked$dm."
#define BOSS_CURED_FROM_PARALIZED_MSG "\tEn garde! %s is no longer $eyparalized$dy."

typedef enum {START=0, WINS=1, LOSES=2, MID_FIGHT=3, END=4, RAN=5}msg;
#define MAX_TEXT_MSG 256
#define BOSS_MSG_N 6

/*BILIAR*/

#define BILIAR_START_MSG "\t Halt, you mindless programmer! Learn the power of MATH!"
#define BILIAR_WINS_MSG "\t You are far too weak for the double degree."
#define BILIAR_LOSES_MSG "\t But.. how?? I was the great mathematician of the EPS..."
#define BILIAR_MID_FIGHT_MSG "\t The power of my mind is starting to make me ache a little bit..."
#define BILIAR_END_MSG "\t Okay... I am craving for some LoL right now."
#define BILIAR_RAND_MSG "\t Linux? Hahahaha, you cannot use that against me, Jay."

/*CYCLOPS*/

#define CYCLOPS_START_MSG "\t You are not going anywhere without having passed CIREL!"
#define CYCLOPS_WINS_MSG "\t Just like all the others. You can't even phasor properly."
#define CYCLOPS_LOSES_MSG "\t You have capability of unleashing great power, young phase. Take my Transistor, and use it to unravel this conflict."
#define CYCLOPS_MID_FIGHT_MSG "\t Oh, no, you don't! DIODES, TO ME!"
#define CYCLOPS_END_MSG "\t *APPROACHING BREAKDOWN VOLTAGE*"
#define CYCLOPS_RAND_MSG "\t I shall divide you in half like any other voltage!"

/*IRONMAN*/

#define IRONMAN_START_MSG "\t I shall not grant you pass to the Secret Math Library, Jay Phi! Turn back!"
#define IRONMAN_WINS_MSG "\t See? You have no power to stand a chance against The Algebra Crow."
#define IRONMAN_LOSES_MSG "\t Perhaps I was wrong about you, Jay. Maybe you can solve the equation and defeat Cuervs."
#define IRONMAN_MID_FIGHT_MSG "\t I see your strategy. It is not enough!"
#define IRONMAN_END_MSG "\t What? Well I didn't see that coming... Let me consult my Russian friend about that approach."
#define IRONMAN_RAND_MSG "\t *shaky shaky*"

/*CUERVS*/

#define CUERVS_START_MSG "\tOh, if it isn't my pitiful minion, Jay. Here, defiant, even though you know you cannot stop the dual space.\n\tBut it does not matter.\n\tHave at thee!"
#define CUERVS_WINS_MSG "\tAAAHAHAHAHAHAHAHAHAHA! You had no chance, you arithmeticless fool!"
#define CUERVS_LOSES_MSG "\tNO, NO, NO! My oh so wonderful dualized space! My beautiful David stars! AAAARRRGGGGHHHHHHHHHH"
#define CUERVS_MID_FIGHT_MSG "\tCease your struggle, you meaningless exponent!"
#define CUERVS_END_MSG "\tI shall destroy you, Jay Phi! I created your own existance in this R-vectorial space!"
#define CUERVS_RAND_MSG "\tThis is as easy as a melon rolling down the hill."

#endif
