#include "utils.h"


int main(int argc, char *argv[]){
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <32_bytes_key> <file_name> <16_bytes_tag>\n", argv[0]);
        return 1;
    }

    char *inputKey = argv[1];
    char *inputTag = argv[3];
    const char *filename = argv[2];

    // Verify the length of the Key
    if ((int)strlen(inputKey) != 64){
        fprintf(stderr,"The Key must be 64 characters long. Current Key has length %d\n", (int)strlen(inputKey));
        return 1;
    }

    // Verify the length of the Tag
    if ((int)strlen(inputTag) != 32){
        fprintf(stderr,"The Tag must be 32 characters long. Current Tag has length %d\n", (int)strlen(inputTag));
        return 1;
    }

    mpz_t given, computed;
    unsigned char given_byte_tag[16];
    unsigned char computed_byte_tag[16];

    // Get input tag and convert it to mpz_t
    getTag(inputTag, given_byte_tag);
    leBytesToNum(given_byte_tag, 16, given);

    // Compute real tag using Poly1305 and convert the string result to a mpz_t
    char *computed_tag = Poly1305(inputKey, filename);
    getTag(computed_tag, computed_byte_tag);
    leBytesToNum(computed_byte_tag, 16, computed);

    // gmp_printf("given    = %Zx\n", given);
    // gmp_printf("computed = %Zx\n", computed);

    // Compare both mpz_t
    if(!mpz_cmp(given, computed))
        printf("ACCEPT\n");
    else
        printf("REJECT\n");

    return 0;
}