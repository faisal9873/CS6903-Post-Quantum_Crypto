#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>      // close, write
#include <time.h>        // time, ctime
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM,
                         // bind, listen, accept
#include <netdb.h>     // getaddrinfo, NI_MAXHOST, NI_MAXSERV
#include <signal.h>
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/select.h>
#include <signal.h> /* sigaction, SIGINT, SIGQUIT */
#include <errno.h>
#include "chat.h"
#include "ntru/api.h"

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif


#define LOCAL_ADDR "127.0.0.1"
#define DEFAULT_PORT "5000"

void key_exchange(int serverfd, unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key );
void connect_to_server(struct addrinfo * result, unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key);
void establish_connection(unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key);

#endif