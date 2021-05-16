#include <stdlib.h>
#include <stdio.h>

#include <libdbuf.h>

//You can use test unit libraries such as...
//#include "CUnit/Basic.h"


int main(int args, char** argv) {
    int rval = EXIT_SUCCESS;

    printf("Creating new buffer...\n");
    dbuf* buffer = dbuf_create(10);

    printf("Filling buffer...\n");
    char string[] = "Hello, World!";
    for (int i = 0; i < sizeof(string); i++) {
        dbuf_append(buffer, string[i]);
    }
    printf("Opening file...\n");
    FILE* f = fopen("dbuffer.bin", "wb");

    printf("Writing buffer...\n");
    dbuf_serialize(buffer, f);

    printf("Closing file...\n");
    fclose(f);

    printf("Reopening file...\n");
    f = fopen("dbuffer.bin", "rb");

    printf("Deserializing...\n");
    dbuf* buffer2 = dbuf_deserialize(f);

    printf("Comparing buffer length...\n");
    if (dbuf_length(buffer) != dbuf_length(buffer2)) {
        rval = EXIT_FAILURE;
    }

    printf("Cleaning up...\n");
    dbuf_destroy(buffer);
    dbuf_destroy(buffer2);
    
    return rval;
}
