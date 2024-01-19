#include <stdio.h>
#include "gmp.h"


int main(int argc, char *argv[]){
    // if (argc != 2) {
    //     fprintf(stderr, "Usage: %s <32_bytes_key> <file_name>\n", argv[0]);
    //     return 1;
    // }

    mpz_t N;
    mpz_init(N);

    mpz_set_ui(N, 1);

    gmp_printf("hello N=%Zd\n", N);

    mpz_clear(N);
    
    return 0;
}