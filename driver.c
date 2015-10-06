/*-----------------------------------------------------------------------------
 *                                DRIVER.C
 *-----------------------------------------------------------------------------
 * in its current form this simulation simulates a DIRECT MAP cache
 *
 * addresses are stored on per line in either hex or decimal form and are
 * expected to be no greater than 32 bit values
 *
 * the log base 2 of both the cache size and block size are arguments one and
 * two respectively of the command with argument three being a flag for
 * toggling tracing on or off
 * 
 * the forth argument is the target file containing the addresses
 *
 * AUTHOR: elijah
 */

#include    "driver.h"

int main(int args, char* argv[])
{

    // initalize variables for tracking block and cache size, whether we
    // trace output and then analyze the arguments
    int cachesz, blocksz, traceflag;
    process_args(args, argv, &cachesz, &blocksz, &traceflag);

    // open target file and exist if unable to do so
    FILE *in = fopen(argv[4], "r");
    if (in == NULL){
        fprintf(stderr, "FATAL ERROR: could not open %s\n", argv[4]);
        fprintf(stderr, "SIMULATION TERMINATED\n");
        exit(1);
    }

    #ifdef  DEBUG	
        fprintf(stderr, "CACHE SIZE:\n        %d\n", cachesz);
        fprintf(stderr, "BLOCK SIZE:\n        %d\n", blocksz);
        fprintf(stderr, "TRACE FLAG:\n");
        if (traceflag)
            fprintf(stderr, "        ON\n");
        else
            fprintf(stderr, "        OFF\n");
        fprintf(stderr, "TARGET FILE:\n        %s\n\n", argv[4]);

        fprintf(stderr, "CREATING CACHE STRUCT\n\n");
    #endif

    // initialize cache struct and terminate if it can't be done
    struct cache *newcache;
    newcache = init_cache(cachesz, blocksz);
    if (newcache == NULL){
        fprintf(stderr, "FATAL ERROR: could not initialize cache\n");
        fprintf(stderr, "SIMULATION TERMINATING\n");
        fclose(in);
        exit(1);
    }

    #ifdef  DEBUG
        fprintf(stderr, "CACHE STRUCT CREATED SUCCESSFULLY\n\n");
    #endif

    // a buffer for holding each line and one for storing hit/miss result
    char buff[1028], result[5], cachetag[9];

    // our ints for keeping track of hits, misses and accesses
    int hits, misses, accesses;
    hits = misses = accesses = 0;

    // float for keeping track of miss ratio
    float missratio;

    // print the header line for our output if the trace flag is set
    if (traceflag){
        printf("  address      tag    block cache tag hit/miss hits misses");
        printf(" accesses  miss ratio\n");
    }

    #ifdef  DEBUG
        int loop = 0;
    #endif

    // loop as long as lines can be read
    while (fscanf(in, "%s", buff) != EOF){

        #ifdef  DEBUG
            fprintf(stderr, "TOKEN %d:\n\t%s\n", ++loop, buff);
        #endif

        // read in new address from line bufferg and ensure it was procesed
        // correctly
        struct address *currentaddress;
        currentaddress = init_address(newcache, get_address(buff));
        if (currentaddress == NULL){
            fprintf(stderr, "FATAL ERROR: could not process address\n");
            fprintf(stderr, "SIMULATION TERMINATING\n");
            free_cache(newcache);
            fclose(in);
            exit(1);
        }

        // allocate block for holding our lookup information
        struct block *hold;
        hold = init_block();
        if (hold == NULL){
            fprintf(stderr, "FATAL ERROR: could not initialize block\n");
            fprintf(stderr, "SIMULATION TERMINATING\n");
            free_cache(newcache);
            free(currentaddress);
            fclose(in);
            exit(1);
        }

        // check to see if we located the current address block and tag in our
        // cache
        if (cache_lookup(newcache, hold, currentaddress)){

            // an evaluation of true means we did not locate it so we
            // increment the miss counter
            misses++;

            // new we check to see if the block located is written and if so
            // we copy the tag to cachetag buffer and if not, we make it blank
            if (hold->iswritten)
                sprintf(cachetag, "%lx", hold->tag);
            else
                sprintf(cachetag, "");
            strcpy(result, "miss");

            // because our data is not in cache we need to write it
            if (write_to_cache(newcache, currentaddress)){
                fprintf(stderr, "FATAL ERROR: could not write to cache\n");
                fprintf(stderr, "SIMULATION TERMINATING\n");
                free_cache(newcache);
                free(hold);
                free(currentaddress);
                fclose(in);
                exit(1);
            }
        } else {

            // our data was located successfully so we increment the hit
            // counter and copy the tag
            hits++;
            sprintf(cachetag, "%lx", hold->tag);
            strcpy(result,"hit");
        }
	
        // increment the access counter and re-calculate the miss ratio
        accesses++;
        missratio = (float) misses / (float) accesses;

        // if the traceflag is set we print out the trace information
        if (traceflag){

            #ifdef  DEBUG
                fprintf(stderr, "TRACE OUTPUT:\n");
                fprintf(stderr, "%9lx%9lx%9lx%10s%9s%5d%7d%9d%12.8f\n\n",
                       currentaddress->raw, currentaddress->tag,
                       currentaddress->index, cachetag, result,hits, misses,
                       accesses, missratio);
            #endif

            printf("%9lx%9lx%9lx%10s%9s%5d%7d%9d%12.8f\n", currentaddress->raw,
                   currentaddress->tag, currentaddress->index, cachetag, result,
			       hits, misses, accesses, missratio);
        }

        // free hold and the current address structs
        free(hold);
        free(currentaddress);
    }

    // free the cache struct
    free_cache(newcache);

    // close the input file
    fclose(in);

    // print requisite simulation information
    printf("%s\n", NAME);
    int i;
    for (i = 1; i < args; i++)
        printf("%s ", argv[i]);
    printf("\n");
    printf("memory accesses: %d\n", accesses);
    printf("hits: %d\n", hits);
    printf("misses: %d\n", misses);
    printf("miss ratio: %.8f\n", missratio);

    return 0;
}

/**
 * determines if token is a hex or int and assigns to value and returns
 *
 * PARAMETERS: char*
 */
unsigned long get_address(char* token)
{
    unsigned long value;
    if (token[1] == 'x')
        sscanf(token, "%lx", &value);
    else
        sscanf(token, "%lu", &value);
    return value;
}

/**
 * takes command string, argcount, and variables for the cache, block and
 * trace values and parses string assigning correct values to each
 *
 * no structs have been initlaized or files open at this point so this function
 * is given the ability to exit independently of the main function without 
 * returning
 *
 * PARAMETERS: int, char*, int*, int*, int*
 */
void process_args
(
    int argcount,
    char* arguments[],
    int *cache, int *block,
    int *trace
)
{
    #ifdef  DEBUG
        fprintf(stderr, "PROCESSING ARGUMENTS:\n");
        fprintf(stderr, "        ARGUMENTS:\n");
        fprintf(stderr, "                ");
        int i;
        for(i = 0; i < argcount; i++)
            fprintf(stderr, "%s ", arguments[i]);
        fprintf(stderr, "\n        ARGUMENT COUNT: %d\n\n", argcount);
    #endif

  	// verify correct number of arguments and print error msg and exit if not
    // correct
    if (argcount != 5){
        fprintf(stderr, "FATAL ERROR: usage error\n");
        fprintf(stderr, "PROPER USAGE:\n");
        fprintf(stderr, "\tdriver [int] [int] [flag = on/off] [input file]\n");
        fprintf(stderr, "SIMULATION TERMINATED\n");
        exit(1);
    }

    // verify that both cache and block arguments are not negative
    *cache = atoi(arguments[1]);
    *block = atoi(arguments[2]);
    if (*cache < 0 || *block < 0){
        fprintf(stderr, "FATAL ERROR: invalid cache or block size\n");
        fprintf(stderr, "INFORMATION: cache and block size must be integers ");
        fprintf(stderr, "equal to or greater than zero\n");
        fprintf(stderr, "SIMULATION TERMINATED\n");
        exit(1);
    }

    // verify that cache size is not smaller than block size
    if (*cache < *block){
        fprintf(stderr, "ERROR: invalid cache or block size\n");
        fprintf(stderr, "INFORMATION: cache size must be equal to or greater ");
        fprintf(stderr, "than block size\n");
        fprintf(stderr, "PROGRAM TERMINATED\n");
        exit(1);
    }

    // raise two to argument values ot get final size
    *cache = pow(2, *cache);
    *block = pow(2, *block);
	 

    // verify that on/off flag is set correctly and store in traceflag pointer
    if (strcmp(arguments[3], "on") == 0){
        *trace = 1;
    } else if (strcmp(arguments[3], "off") == 0){
        *trace = 0;;
    } else {
        fprintf(stderr, "ERROR: invalid trace flag argument\n");
        fprintf(stderr, "INFORMATION: trace flag must be set to either \"on\"");
        fprintf(stderr, " or \"off\"\n");
        fprintf(stderr, "PROGRAM TERMINATED\n");
        exit(1);
    }
}

