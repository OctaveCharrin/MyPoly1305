#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"
#include "utils.h"


int main(int argc, char *argv[]){
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <32_bytes_key> <file_name> <16_bytes_tag>\n", argv[0]);
        return 1;
    }

    char const *inputKey = argv[1];
    char const *inputTag = argv[3];
    char const *filename = argv[2];

    // Verify the length of the Key
    size_t keyLength = strlen(inputKey);
    if (keyLength != 64) {
        fprintf(stderr, "The Key must be 64 characters long. Current Key has length %zu\n", keyLength);
        return 1;
    }

    // Verify the length of the Tag
    size_t tagLength = strlen(inputTag);
    if (tagLength != 32) {
        fprintf(stderr, "The Tag must be 32 characters long. Current Tag has length %zu\n", tagLength);
        return 1;
    }

    mpz_t given, computed;
    mpz_inits(given, computed, NULL);
    unsigned char given_byte_tag[16];
    unsigned char computed_byte_tag[16];

    for (int i=0; i<16; ++i){
        given_byte_tag[i] = 0;
        computed_byte_tag[i] = 0;
    }

    // Get input tag and convert it to mpz_t
    getTag(inputTag, given_byte_tag);
    leBytesToNum(given_byte_tag, 16, given);

    // Compute real tag using Poly1305 and convert the resulting string to mpz_t
    char *computed_tag = Poly1305(inputKey, filename);
    getTag(computed_tag, computed_byte_tag);
    leBytesToNum(computed_byte_tag, 16, computed);

    // gmp_printf("given    = %Zx\n", given);
    // gmp_printf("computed = %Zx\n", computed);

    // Compare both mpz_t
    if(mpz_cmp(given, computed) == 0)
        printf("ACCEPT\n");
    else
        printf("REJECT\n");
    mpz_clears(given, computed, NULL);
    free(computed_tag);
    return 0;
}