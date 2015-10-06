/*------------------------------------------------------------------------------
 *                                ADDRESS.C                                     
 *------------------------------------------------------------------------------
 * contains functions required for handling address struct
 *
 * AUTHOR: blind-guy
 */

#include	"address.h"

/**
 * creates and fills an address struct based on information provided by a
 * cache struct and the raw value given, should return pointer to address or
 * NULL on failure
 */
struct address *init_address(struct cache *mycache, unsigned long value)
{
	// malloc space of address and return NULL if it fails
	struct address *temp = malloc(sizeof(struct address));
	if (temp == NULL){
		fprintf(stderr, "ERROR: not able to malloc address\n");
		return NULL;
	}

	// set mem to 0 value bytes
	memset(temp, 0, sizeof(struct address));
	
	// set the raw address member
	temp->raw = value;

	// mathy bits
	temp->offset = temp->raw % mycache->blocksize;
	temp->blockaddress = temp->raw / mycache->blocksize;
	temp->index = temp->blockaddress % mycache->blockcount;
	temp->tag = temp->blockaddress / mycache->blockcount;

	return temp;
}
