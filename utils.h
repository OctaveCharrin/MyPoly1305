// utils.h

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED


/**
 * Converts an buffer of bytes stored in little endian to mpz_t integer.
 * @param in The input bytes buffer.
 * @param len The input buffer length.
 * @param out The output mpz_t integer.
 */
void leBytesToNum(unsigned char const *in, int const len, mpz_t out);

/**
 * Converts the input 64 hexadecimal character string to a 32 bytes Key.
 * @param input The input hexadecimal string.
 * @param key The output little endian Key.
 */
void getKey(char const *input, unsigned char *key);

/**
 * Converts the input 64 hexadecimal character string to a 16 bytes Tag.
 * @param input The input hexadecimal string.
 * @param key The output little endian Tag.
 */
void getTag(char const *input, unsigned char *tag);

/**
 * Splits the 32 bytes Key into two 16 bytes subkeys, and clamps r to
 * match RFC 8439 specifications.
 * @param key The input key.
 * @param r The buffer to write the clamped subkey r.
 * @param s The to write the subkey s.
 */
void getRS(unsigned char const *key, unsigned char *r, unsigned char *s);

/**
 * Prints the hexadecimal representation of a buffer bytes per bytes.
 * @param key The buffer to print.
 * @param len The number of bytes to print.
 */
void printBytes(unsigned char const *buffer, int const len);

/**
 * Computes the tag of the file using a key specified as a little endian
 * hexadecimal key.
 * @param inputKey The input hexadecimal key used for authentication.
 * @param filename The name of the file to authenticate.
 * @return The hexadecimal representation of the resulting tag as a string.
 */
char * Poly1305(char const *inputKey, char const *filename);

#endif // UTILS_H_INCLUDED