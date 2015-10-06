/*-----------------------------------------------------------------------------
 *                                CACHE.C
 *-----------------------------------------------------------------------------
 * contains source for functions necessary for handling a cache
 *
 * AUTHOR: elijah
 */

#include    "cache.h"

/**
 * builds cache using parameters representing cache size and block size and
 * returns a pointer to cache struct or NULL on failure
 */
struct cache *init_cache(unsigned long cachesz, unsigned long blocksz)
{

	#ifdef  DEBUG
		fprintf(stderr, "BUILDING CACHE:\n        SIZE: %lu\n", cachesz);
        fprintf(stderr, "        BLOCKS: %lu\n\n", blocksz);
	#endif

	// attempt to malloc temp struct and return NULL on failure
	struct cache *temp = malloc(sizeof(struct cache));
	if (temp == NULL){
		fprintf(stderr, "ERROR: could not malloc cache\n");
		return NULL;
	}

	// clean up memory to make sure we have no garbage stored
	memset(temp, 0, sizeof(struct cache));

	temp->size = cachesz;
	temp->blocksize = blocksz;
	temp->blockcount = temp->size / temp->blocksize;

	// allocate memory for our blocks and if it fails, free temp and return
	// NULL
	temp->blocks = malloc(sizeof(struct block) * temp->blockcount);
	if (temp->blocks == NULL){
		fprintf(stderr, "ERROR: cold not malloc blocks in cache\n");
		free(temp);
		return NULL;
	}
	
	clean_cache(temp);

	return(temp);
}

/**
 * given a cache struct we reset the memory held by the blocks member and reset
 * the iswritten field of each block to FALSE
 *
 * PARAMETERS: struct cache*
 */
void clean_cache(struct cache *mycache)
{
	// there's no use cleaning a non-existent cache or one which points to
	// no blocks
	if(mycache == NULL || mycache->blockcount == 0 || mycache->blocks == NULL)
		return;

	// set all mem pointed to by mycache->blocks to 0
	memset(mycache->blocks, 0, sizeof(struct block) * mycache->blockcount);

	// set all iswritten fields to FALSE
	struct block *current;
	current = mycache->blocks;
	int i;
	for(i = 0; i < mycache->blockcount; i++){
		current->iswritten = 0;
		current++;
    }
}


/**
 * given a cache we free it
 *
 * PARAMETERS: struct cache*
 */
void free_cache(struct cache *mycache)
{
	free(mycache->blocks);
	free(mycache);
}

/**
 * Given an address and cache we determine if the block the address refers to
 * exists in the cache. If the block is located successfully we return 0 else
 * return a non-zero value. In both cases the struct block's members are set
 * to those of what's in memory.
 *
 * PARAMETERS: struct cache*, struct block*, struct address*
 */
int cache_lookup
(
	struct cache *mycache,
	struct block *hold,
	struct address *myaddress
)
{
	#ifdef  DEBUG
        fprintf(stderr, "LOOKING UP ADDRESS: %lx\n", myaddress->raw);
        fprintf(stderr, "\tTAG:   %lx\n", myaddress->tag);
        fprintf(stderr, "\tINDEX: %lx\n", myaddress->index);
	#endif

	// first set our temp struct to match the index of our address
	struct block *temp = mycache->blocks + myaddress->index;

	#ifdef DEBUG
        fprintf(stderr, "BLOCK AT INDEX: %lx\n", myaddress->index);
        if (!temp->iswritten)
            fprintf(stderr, "\tIS NOT WRITTEN\n");
        else{
            fprintf(stderr, "\tIS WRITTEN\n");
            fprintf(stderr, "\tTAG: %lx\n", temp->tag);
        }
    #endif

	// check to see if the block it points to is written
	if (temp->iswritten){
		// if it is we copy the tag and iswritten value to our hold
		hold->tag = temp->tag;
		hold->iswritten = temp->iswritten;

		// if the tags match we return 0 to indicate success else non-zero
		if (temp->tag == myaddress->tag){
			#ifdef  DEBUG
                fprintf(stderr,"TAG MATCH\n");
			#endif
			return 0;
		} else {
            #ifdef  DEBUG
                fprintf(stderr, "TAG MISMATCH\n");
            #endif
			return 1;
		}
	}

	// the block we found was not written so hold gets no values and we
	// return one
	return 1;
}

/**
 * given a cache and address we write to the block specified by the address
 * index that is we simply replace whatever tag is currently storred in the
 * block
 *
 * returns 0 if we could write to cache or non-zero if it fails
 *
 * PARAMETERS: struct cache*, struct address*
 */
int write_to_cache(struct cache *mycache, struct address *myaddress)
{
    #ifdef  DEBUG
        fprintf(stderr, "ATTEMPTING TO WRITE TO CACHE\n");
    #endif

	// first we verify if we can write to the cache by checking if it exists,
	// if the address exists, and if the index is within our range
	if (mycache == NULL){
		fprintf(stderr, "ERROR: passed NULL cache struct to ");
		fprintf(stderr, "write_to_cache()t\n");
		return 1;
	} else if (myaddress == NULL){
		fprintf(stderr, "ERROR: passed NULL address struct to ");
		fprintf(stderr, "write_to_cache()\n");
		return 1;
	} else if (myaddress->index > mycache->blockcount){
		fprintf(stderr, "ERROR: address struct index excededs cache ");
		fprintf(stderr, "blockcount\n");
		return 1;
	}

	#ifdef  DEBUG
        fprintf(stderr, "WRITEN TAG %lx TO BLOCK %lx\n", myaddress->tag,
                myaddress->index);
    #endif

	struct block *temp;
	temp = mycache->blocks + myaddress->index;
	temp->tag = myaddress->tag;
	temp->iswritten = 1;

	return 0;
}
	
