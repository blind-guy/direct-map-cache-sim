# direct-map-cache-sim
This is a simple direct map cache simulator.

The program accepts four arguments, two integers representing the log base 2 of a cache and block size respectively, a trace flag argument, either `on` or `off`, and a target file.

The  target file is assumed to contain 32 bit addresses written in either decimal or hex with the latter having leading 0x. It is assumed that there is no more than one address per line in the input file. See the example data files.

If the trace flag is "off" output includes on each line and in this order:  
-the name of the usr who ran the simulator which can be changed before compiling in driver.  
-the 4 command line arguments excluding the program name  
-the number of memory accesses  
-the number of hits  
-the number of misses  
-the miss ratio  

If the trace flag is set to "on" a heading containing the following line is printed:
>  address      tag    block cache tag hit/miss hits misses accesses  miss ratio  
This is followed by a line of text for every processed address containing the above information for that address. The address, tag, block and cache tag fields are all printed in hex without the leading 0x.

For our purposes we are assuming a direct cache meaning our model assumes a cache is made of blocks. where each block index maps to exactly one location in the cache. This contrasts with an n-associative cache where a block index maps to a set in cache containing n blocks each. Every address line in the file simply referes to a memory access and does not specifiy a read or write nor is this needed for our approach as both are assumed to result in a hit if the data is present and a miss if it is not.


