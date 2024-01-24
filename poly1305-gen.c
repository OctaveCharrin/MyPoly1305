#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"
#include "utils.h"


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <32_bytes_key> <file_name>\n", argv[0]);
        return 1;
    }

    char const *inputKey = argv[1];
    char const *filename = argv[2];

    // Verify the length of the key
    size_t keyLength = strlen(inputKey);
    if (keyLength != 64) {
        fprintf(stderr, "The Key must be 64 characters long. Current Key has length %zu\n", keyLength);
        return 1;
    }

    char *tag = Poly1305(inputKey, filename);
    printf("%s\n", tag);

    free(tag);
    return 0;
}