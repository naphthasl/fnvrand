#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "src/radix.h"

/* THIS IS FOR TESTING PURPOSES ONLY. PLEASE EXCUSE THE MESS. */

int main(int argc, char *argv[])
{
    RadixTable test_table = RadixTable_New();
    RadixTable_SetItem(&test_table, StrToBlob("hello"), StrToBlob("world"));
    RadixTable_SetItem(&test_table, StrToBlob("itw2"), StrToBlob("agjojeofoajgfjjj"));
    printf("TableTest: %s \n", (char *)RadixAbstract_GetBlobPointer(*RadixTable_GetItem(&test_table, StrToBlob("hello"))));
    RadixTable_SetItem(&test_table, StrToBlob("hello"), StrToBlob("bar"));
    printf("TableTest: %s \n", (char *)RadixAbstract_GetBlobPointer(*RadixTable_GetItem(&test_table, StrToBlob("hello"))));
    RadixTable_DestroyItem(&test_table, StrToBlob("hello"));
    printf("TableTest: %s \n", (char *)RadixAbstract_GetBlobPointer(*RadixTable_GetItem(&test_table, StrToBlob("itw2"))));
    RadixTable_DestroyTable(&test_table);

    printf("Contains: %llu \n", RadixAbstract_BlobContains(StrToBlob("hello world"), StrToBlob("o wo"), 4));

    uint64_t kts = fnv2r64("Tast", 4);
    printf("KTS: "); u_printmemhex(&kts, 8); printf("\n");

    char *key = "readytogo";
    RadixMemoryBlob bkey = (RadixAbstract_ConstructPointerBlob(key, strlen(key)));
    uint64_t hkey = cr_keygen(bkey, (uint64_t)time(0), 16777216);
    printf("KEY: %016llX\n", (long long unsigned int)hkey);

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

        bdata = RadixAbstract_ConstructPointerBlob(argv[i], length);
        cr_encrypt_decrypt(bdata, hkey);
        printf("ENC: "); u_printmemhex(argv[i], length); printf("\n");

        cr_encrypt_decrypt(bdata, hkey);
        printf("DEC: "); u_printmemhex(argv[i], length); printf("\n");

        RadixAbstract_DestroyBlob(&bdata);

        printf("FIN: %s\n", argv[i]);
    }

    return 0;
}
