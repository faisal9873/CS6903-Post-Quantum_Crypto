#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

/*
https://github.com/bawejakunal/AES-CBC-256/blob/master/aes.c

*/

void sym_handle_errors(void);
int sym_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);

int sym_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

#endif