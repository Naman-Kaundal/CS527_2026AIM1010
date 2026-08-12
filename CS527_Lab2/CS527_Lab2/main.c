#include <stdio.h>
#include "compiler.h"
#include "processor.h"
#include "memory.h"

int main()
{
    compile("program.txt");

    initialize();
    reset();

    while (!end_of_simulation) {
        fetch();
        decode();
        execute();
    }

    finalize();

    return 0;
}