/*-----------------------------------------------------------------------------
 *                                DRIVER.H
 *-----------------------------------------------------------------------------
 * contians includes and function prototypes for driver.c
 *
 * AUTHOR: blind-guy
 */

#ifndef     DRIVER_H
    #define     DRIVER_H

    #include	<stdio.h>
    #include	<stdlib.h>
    #include	<math.h>
    #include	<string.h>

    #include	"cache.h"

	// change name of printed name of user who ran the scenario
	#define     NAME   "blind-guy"

    unsigned long get_address(char *);
    void process_args(int, char**, int*, int*, int*);
#endif

