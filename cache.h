/*-----------------------------------------------------------------------------
 *                                  CACHE.H
 *-----------------------------------------------------------------------------
 * contains struct definition and function prototypes for the cache struct
 *
 * AUTHOR: blind-guy
 */

#ifndef     CACHE_H
    #define     CACHE_H

    #include    "block.h"
    #include    "address.h"
	#include    "debug.h"

    #define     TRUE    1
    #define     FALSE   0

    /**
     * at its most basic a cache is simply a set of memory blocks where the cache
     * size, block size, block count are set
     *
     * for our cache we will preserve these values as members and have a member
     * that points to a block of memory which contains our blocks, represented by
     * a block struct
     */
    struct cache{
        unsigned long size;
        unsigned long blocksize;
        unsigned long blockcount;
        struct block *blocks;
    };

    struct address;

    struct cache *init_cache(unsigned long, unsigned long);
    void free_cache(struct cache*);
    int cache_lookup(struct cache*, struct block*, struct address*);
    void clean_cache(struct cache*);
    int write_to_cache(struct cache*, struct address*);

#endif
