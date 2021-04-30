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

#define max(a,b) (a > b ? a : b)
#define MESSAGE_LENGTH 1024

void establish_signal_handlers(struct sigaction* saptr);
void chat(int clientfd, unsigned char* sshared_key);

#endif