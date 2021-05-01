#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>       // perror, snprintf
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <signal.h>
#include <errno.h>
#include "symmetric.h"
#include "print_hex.h"
#include "ntru/rng.h"
#include "ntru/api.h"
#include "ntru/fips202.h"

#define max(a,b) (a > b ? a : b)

#define SEQUENCE_BYTES 4
#define IV_BYTES 16
#define CIPHER_BYTES 255
#define CIPHER_LENGTH_BYTES 1
#define MESSAGE_BYTES CIPHER_BYTES
#define TAG_BYTES 32
#define TOTAL_CIPHERTEXT_BYTES (SEQUENCE_BYTES + IV_BYTES + CIPHER_BYTES + CIPHER_LENGTH_BYTES + TAG_BYTES)

void establish_signal_handlers(struct sigaction* saptr);
void chat(int clientfd, unsigned char* sshared_key);

#endif