#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crypt.h"

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        printf("usage: fnvrand-test ARGUMENTS\n");
        return 1;
    }

    char *str;
    for (i = 1; i < argc; i++)
    {
        /*
        str = (char*)malloc(strlen(argv[i]));
        memcpy(str, argv[i], strlen(argv[i]));
        cr_encrypt_decrypt(str, strlen(str), "readytogo", 9);
        cr_encrypt_decrypt(str, strlen(str), "readytogo", 9);
        printf("%s \"%s\"\n", str, argv[i]);
        free(str);
        */

       cr_encrypt_decrypt(argv[i], strlen(argv[i]), "readytogo", 9);
       cr_encrypt_decrypt(argv[i], strlen(argv[i]), "readytogo", 9);
       printf("%s\n", argv[i]);
    }

    return 0;
}
