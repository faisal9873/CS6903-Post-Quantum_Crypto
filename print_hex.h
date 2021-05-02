#ifndef PRINT_HEX_H
#define PRINT_HEX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
/*
https://ntru.org/resources.shtml
ntru-hps4096821/PQCgenKAT_kem.c fprintBstr method
*/
#define ull unsigned long long
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x80 ? '1' : '0')

void
print_hex(FILE *fp, char *S, unsigned char *A, unsigned long long L);
unsigned long byte_to_bit(unsigned char* byte_arr);
void bit_to_byte(unsigned long bit, unsigned char* byte_arr);
void cyclic_incr_byte_arr(unsigned char* byte_arr);
bool compare_byte_arr(unsigned char* byte_arr1, unsigned char* byte_arr2, int n);

#endif