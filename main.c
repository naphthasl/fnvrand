#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "radix.h"

int main(int argc, char *argv[])
{
    char *key = "readytogo";
    RadixMemoryBlob bkey = (RadixAbstract_ConstructCOWBlob(key, strlen(key)));
    uint64_t hkey = cr_keygen(&bkey, (uint64_t)time(0), 16777216);
    printf("KEY: %016llX\n", hkey);

    if (argc < 2)
    {
        printf("usage: fnvrand-test ARGUMENTS\n");
        return 1;
    }

    RadixMemoryBlob bdata;

    int i; for (i = 1; i < argc; i++)
    {
        int length = strlen(argv[i]);
        printf("IN : "); u_printmemhex(argv[i], length); printf("\n");

        bdata = RadixAbstract_ConstructCOWBlob(argv[i], length);
        cr_encrypt_decrypt(&bdata, hkey);
        printf("ENC: "); u_printmemhex(argv[i], length); printf("\n");

        cr_encrypt_decrypt(&bdata, hkey);
        printf("DEC: "); u_printmemhex(argv[i], length); printf("\n");

        printf("FIN: %s\n", argv[i]);
    }

    return 0;
}
