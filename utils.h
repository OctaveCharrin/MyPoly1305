#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"


char hexToByte(char hexDigit);
void leBytesToNum(unsigned char *in, int len, mpz_t out);
void numToLeHexString(mpz_t in, char *out, int len);

void getKey(char *input, unsigned char *key);
void getTag(char *input, unsigned char *tag);
void getRS(unsigned char *key, unsigned char *r, unsigned char *s);
void printBytes(unsigned char *key, int len);

char* Poly1305(char *inputkey, const char *filename);