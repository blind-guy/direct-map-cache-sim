/*-----------------------------------------------------------------------------
 *                                  BLOCK.C
 *-----------------------------------------------------------------------------
 * contains functions for handling block struct
 *
 * AUTHOR: blind-guy
 */

#include	"block.h"

/**
 * a block is initialized and pointer returned after all bytes are zeroed out
 * or NULL is returned on failure
 */
struct block *init_block()
{
	struct block *temp = malloc(sizeof(struct block));
	if (temp == NULL){
		fprintf(stderr, "ERROR: could not malloc block\n");
		return(NULL);
	}

	memset(temp, 0, sizeof(struct block));
	temp->iswritten = 0;

	return(temp);
}

