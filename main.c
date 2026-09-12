#include "os.h"

/*
 * If a program/data pair is given on the command line it is loaded as the
 * very first task (handy for non-interactive testing), then the OS takes
 * over
 */
int main(int argc, char **argv)
{
    if (argc >= 3) {
        loader(argv[1], argv[2]);
    }

    run_os();

    return 0;
}
