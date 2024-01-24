#include "utils.h"


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <32_bytes_key> <file_name>\n", argv[0]);
        return 1;
    }

    char *inputKey = argv[1];
    const char *filename = argv[2];

    // Verify the length of the key
    if ((int)strlen(inputKey) !=64){
        fprintf(stderr,"The Key must be 64 characters long. Current Key has length %d\n", (int)strlen(inputKey));
        return 1;
    }

    char *tag = Poly1305(inputKey, filename);
    printf("%s\n", tag);

    return 0;
}