#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crypt.h"

int main(int argc, char *argv[])
{
    const char key[] = "readytogo";

    if (argc < 2)
    {
        printf("usage: fnvrand-test ARGUMENTS\n");
        return 1;
    }

    int i; for (i = 1; i < argc; i++)
    {
        cr_encrypt_decrypt(argv[i], strlen(argv[i]), key, strlen(key));
        cr_encrypt_decrypt(argv[i], strlen(argv[i]), key, strlen(key));
        printf("%s\n", argv[i]);
    }

    return 0;
}
