#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>       // perror, snprintf
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM,
                         // bind, listen, accept
#include <netdb.h>     // getaddrinfo, NI_MAXHOST, NI_MAXSERV
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/select.h>
#include <signal.h> /* sigaction, SIGINT, SIGQUIT */
#include <errno.h>
#include "chat.h"
#include "api.h"
#include "print_hex.h"
#include "time_helper.h"


#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

#define LOCAL_ADDR "127.0.0.1"
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
#define DEFAULT_PORT "5000"
#define BACKLOG 50

void key_exchange(int clientfd, unsigned char* cipher, unsigned char* sshared_key);
void print_usage();
void wait_for_client(int listenfd, struct sockaddr_storage* client_addr, char* host, 
                    char* service, char* addr_str, unsigned char* cipher, unsigned char* sshared_key);
int find_ip_addr_and_listen(struct addrinfo * result);
void establish_connection(unsigned char* cipher, unsigned char* sshared_key);

#endif
