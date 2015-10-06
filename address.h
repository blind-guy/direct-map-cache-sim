/*-----------------------------------------------------------------------------
 *                                ADDRESS.H                                    
 *-----------------------------------------------------------------------------
 * contains definition and function prototypes for address struct
 *
 * AUTHOR: blind-guy
 */

#ifndef     ADDRESS_H
    #define     ADDRESS_H

    #include    <string.h>
    #include    <stdio.h>
    #include    <stdlib.h>

    #include    "cache.h"
    #include    "debug.h"

    /**
     * broken down into its component parts an address contains three groups of
     * bits each represented by a member of our struct:
     *        TAG | INDEX   | OFFSET
     * however to aid in some math later, we also store the composite value
     * of the TAG and INDEX bits which together are refered to as the block
     * address:
     *        BLOCK ADDRESS | OFFSET
     * and to assist with our simulation we'll also store the raw address value
     */
    struct address{
	    unsigned long raw;
	    unsigned long tag;
	    unsigned long index;
	    unsigned long offset;
	    unsigned long blockaddress;
    };

    struct cache;

    struct address *init_address(struct cache*, unsigned long);
#endif
