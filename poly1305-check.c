#include <stdio.h>
#include "gmp.h"


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <32_bytes_key> <file_name> <16_bytes_tag>\n", argv[0]);
        return 1;
    }
    
    return 0;
}