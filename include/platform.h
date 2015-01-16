#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "world.h"
#include "types.h"

#define PF_DEFAULT_SCREEN stdscr
#define PF_SCREEN_RATIO 60
#define PF_COLLISION_RATIO 120
#define PF_GRAVITY_RATIO 12
#define PF_SPEED_RATIO 120
#define PF_STANCE_RATIO 5
#define PF_BULLETS_RATIO 30
#define PF_MOMENTUM_RATIO 12
#define PF_TOUCH_RATIO 30
#define PF_KILL_RATIO 30
#define PF_THREAD_TIMEOUT 25000

typedef struct _Platform Platform;

/* Initializes and starts platform */
Platform *startPlatform(level lvl, short unlocked_weapons);

/* Waits for platform to stop and returns its*
 * exit code: -1:Error, 0:Success, 1:Dead */
short waitForPlatform(Platform *pf);

/* Destroys platform (must be stopped) */
void destroyPlatform(Platform *pf);

#endif
