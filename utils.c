#include <stdio.h>
#include <stdlib.h>

#include "gmp.h"
#include "utils.h"


#define DEBUG 0


char hexToByte(char const hexDigit) {
    if (hexDigit >= '0' && hexDigit <= '9') {
        return hexDigit - '0';
    } else if (hexDigit >= 'a' && hexDigit <= 'f') {
        return hexDigit - 'a' + 10;
    } else if (hexDigit >= 'A' && hexDigit <= 'F') {
        return hexDigit - 'A' + 10;
    } else {
        fprintf(stderr, "Invalid hexadecimal digit in key: %c\n", hexDigit);
        return 1;
    }
}


void leBytesToNum(unsigned char const *in, int const len, mpz_t out){
    char reversed[2*len+1];
    for (int i=0; i<len+1; ++i){
        snprintf((reversed+2*i), 3, "%02x", in[len-1-i]);
    }
    reversed[2*len] = '\0';
    if (mpz_set_str(out, reversed, 16) == -1) {
        fprintf(stderr, "Error: Invalid hexadecimal string.\n");
    }
}


void numToLeHexString(mpz_t in, char *out, int const len){
    int len2 = len*2;
    int size = (int)mpz_sizeinbase(in, 16);
    char hexString[size+2];

    if (size%2 == 0) {
        mpz_get_str(hexString, 16, in);
    } else {
        hexString[0] = '0';
        mpz_get_str(hexString+1, 16, in);
        size++;
    }

#define min(a, b) ((a) < (b) ? (a) : (b))
    
    int l = min(size, len2);
    for(int i=0; i<l; i+=2){
        out[i] = hexString[size-2-i];
        out[i+1] = hexString[size-1-i];
    }
    for(int i=l; i<len2; ++i){
        out[i] = '0';
    }
    out[len2] = '\0';
}


void printBytes(unsigned char const *key, int const len){
    for (int i=0; i<len-1; ++i){
        printf("%02x:", key[i]);
    }
    printf("%02x\n", key[len-1]);
}


void getKey(char const *input, unsigned char *key){
    for (int i=0; i<32; ++i){
        key[i] = (unsigned char)((hexToByte(input[2*i]) << 4) | hexToByte(input[2*i+1]));
    }
}


void getTag(char const *input, unsigned char *tag){
    for (int i=0; i<16; ++i){
        tag[i] = (unsigned char)((hexToByte(input[2*i]) << 4) | hexToByte(input[2*i+1]));
    }
}


void getRS(unsigned char const *key, unsigned char *r, unsigned char *s){
    for (int i=0; i<16; ++i){
        r[i] = key[i];
        s[i] = key[16 + i];
    }
    // Clamp the resulting r
    r[3] &= 15;
    r[7] &= 15;
    r[11] &= 15;
    r[15] &= 15;
    r[4] &= 252;
    r[8] &= 252;
    r[12] &= 252;
}


char * Poly1305(char const *inputKey, char const * filename){
    
    // Setup
    unsigned char key[32], r[16], s[16];
    getKey(inputKey, key);
    getRS(key, r, s);
    
    mpz_t R, S, P, a, n;
    mpz_inits(R, S, P, a, n, NULL);
    // P = 2^130 - 5
    mpz_set_str(P, "3fffffffffffffffffffffffffffffffb", 16);
    mpz_set_ui(a, 0);
    leBytesToNum(r, 16, R);
    leBytesToNum(s, 16, S);
    
#if DEBUG
    printf("key = ");
    printBytes(key, 32);
    printf("r = ");
    printBytes(r, 16);
    printf("s = ");
    printBytes(s, 16);
    gmp_printf("R = %Zx\n", R);
    gmp_printf("S = %Zx\n", S);
    gmp_printf("P = %Zx\n", P);
#endif

    // Handle the file
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    int counter = 0;
    char byte;
    unsigned char msg[18]; // 16 bytes-block plus the 0x01 byte 
                           // plus the '\0' byte to make a string
    char fileStatus = (byte = fgetc(file));

    while (fileStatus != EOF) {
        msg[counter] = byte;
        counter = (counter+1)%16;
        if (counter == 0){
            msg[16] = 1;
            msg[17] = '\0';
            leBytesToNum(msg, 17, n);

#if DEBUG
            printf("\n******* New Block *******\n");
            printf("The block (with 0x01 byte) is: %s\n", msg);
            printf("which in hex is: ");
            printBytes(msg, 18);
            gmp_printf("leading to n = %Zx\n", n);
            printf("\n");
            gmp_printf("a = %Zx\n", a);
#endif
            mpz_add(a, a, n);
#if DEBUG
            gmp_printf("a+n = %Zx\n", a);
#endif
            mpz_mul(a, a, R);
#if DEBUG
            gmp_printf("(a+n)*r = %Zx\n", a);
#endif
            mpz_mod(a, a, P);
#if DEBUG
            gmp_printf("((a+n)*r)\%P = %Zx\n", a);
#endif

        }

        fileStatus = (byte = fgetc(file));
    }
    fclose(file);

    // Handle the last block
    if (counter != 0){
        msg[counter] = 1;
        msg[counter+1] = '\0';
        leBytesToNum(msg, counter+1, n);

#if DEBUG
        printf("\n******* Last Block *******\n");
        printf("The last block (with 0x01 byte) is: %s\n", msg);
        printf("which in hex is: ");
        printBytes(msg, counter+1);
        gmp_printf("leading to n = %Zx\n", n);
        printf("\n");
        gmp_printf("a = %Zx\n", a);
#endif
        mpz_add(a, a, n);
#if DEBUG
        gmp_printf("(a+n) = %Zx\n", a);
#endif
        mpz_mul(a, a, R);
#if DEBUG
        gmp_printf("(a+n)*r = %Zx\n", a);
#endif
        mpz_mod(a, a, P);
#if DEBUG
        gmp_printf("((a+n)*r)\%P = %Zx\n", a);
#endif

    }

    mpz_add(a, a, S);

    char *tag = (char *)malloc(33*sizeof(char));
    numToLeHexString(a, tag, 16);

#if DEBUG
    gmp_printf("a + s = %Zx\n", a);
    printf("Tag = %s\n", tag);
#endif
    mpz_clears(R, S, P, a, n, NULL);

    return tag;
}