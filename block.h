/*-----------------------------------------------------------------------------
 *                                BLOCK.H
 *-----------------------------------------------------------------------------
 * contains struct definition and function prototypes for the block struct
 *
 * AUTHOR: blind-guy
 */

#ifndef     BLOCK_H
    #define     BLOCK_H

    #include    <string.h>
    #include    <stdlib.h>
    #include    <stdio.h>

    #include    "debug.h"

    /**
     * for our purposes a block will contain its tag and a value determining if
     * it's been written to or not
     */
    struct block{
        unsigned long tag;
        int iswritten;
    };

    struct block *init_block();
#endif
