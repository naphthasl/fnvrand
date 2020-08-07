#include <stdio.h>
#include <string.h>
#include "fnvrand.h"

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        printf("usage: fnvrand-test ARGUMENTS\n");
        return 1;
    }
    
    for (i = 1; i < argc; i++)
        printf("%8X \"%s\"\n", fnv1r(argv[i], strlen(argv[i])), argv[i]);
    
    return 0;
}
