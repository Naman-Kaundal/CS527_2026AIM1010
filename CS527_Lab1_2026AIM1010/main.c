#include <stdio.h>
#include "compiler.h"
#include "memory.h"
#include "processor.h"

/*
 * Compile into bytecode, Load the bytecode and data into memory, Reset the processor,fetch-decode-execute until program ends, write final data back to disk.
 */
int main(void)
{
    compile();    
    initialize(); 
    reset();     

    while (!end_of_simulation) {
        fetch();
        decode();
        execute();
    }

    finalize();   

    printf("Simulation finished successfully.\n");
    return 0;
}