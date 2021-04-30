#include "server.h"


void establish_connection(unsigned char* cipher, unsigned char* sshared_key){
    setbuf(stdout, NULL); // Set buf to null so we don't need to flush
                        // Use C stream and system call simultaneously
    struct sigaction sa;            //Handle SIGINT and SIGQUIT
                                    //to make sure exit is called
                                    //and all file descriptors are closed
    establish_signal_handlers(&sa);


    struct addrinfo hints;
    struct addrinfo * result;

    struct sockaddr_storage client_addr;

    char addr_str[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char* port = DEFAULT_PORT;


    /*
    Prevents server from receiving SIGPIPE
    signal if it tries to write to a socket 
    whose peer has been closed, fail with EPIPE instead
    */

    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
        perror("sigpipe error");
        exit(EXIT_FAILURE);
    }

    /* 
    Create a structure to get all 
    possible ip address and port for 
    to use as server address and store the list
    of possible addresses in result
    */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;//check when this is valid
                                //IPv4 or IPv6
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
                                //Wildcard IP address; service name is numeric

    if(getaddrinfo(NULL, port, &hints, &result) != 0)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    
    /* Listen on one of the workable ip address found from result */
    int listenfd = find_ip_addr_and_listen(result);

    /* Process each client connected to the listening socket */
    wait_for_client(listenfd, &client_addr, host, service, 
                addr_str, cipher, sshared_key);

    
}


void print_usage(){
    char* mess = "./server [name] [port]";
    write(STDOUT_FILENO, mess, strlen(mess));
}

/*
    This function establish connections with clients
    iteratively, one after another for any connected
    client and chat with each client
*/

void wait_for_client(int listenfd, struct sockaddr_storage* client_addr, char* host, 
                    char* service, char* addr_str, unsigned char* cipher, unsigned char* sshared_key){

    socklen_t addrlen;
    int clientfd;
    
    for(;;){

        addrlen = sizeof(struct sockaddr_storage);
        clientfd = accept(listenfd, (struct sockaddr*) client_addr, &addrlen);

        if(clientfd == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /* Get client's IP address and port number */
        if(getnameinfo((struct sockaddr *) client_addr, addrlen,
            host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
            snprintf(addr_str, ADDRSTRLEN, "(%s, %s)", host, service);
        }
        else{
            snprintf(addr_str, ADDRSTRLEN, "(?UNKNOWN?)");
        }

        printf("Connection from %s\n", addr_str);

        key_exchange(clientfd, cipher, sshared_key);
        // if ( (ret_val = crypto_kem_enc(cipher, sshared_key, apublic_key)) != 0) { /* ciphertext, hash, pk, only share ciphertext */
        //     printf("crypto_kem_enc returned <%d>\n", ret_val); /* ss is the shared key */
        //     exit(EXIT_FAILURE);
        // }
        /* wait to receive client's public key */

        /* Starts chatting with server */
        chat(clientfd, sshared_key);

        if(close(clientfd) == -1){
            perror("close serverfd"); 
            exit(EXIT_FAILURE);
        } 
        
    }
}

void key_exchange(int clientfd, unsigned char* cipher, unsigned char* sshared_key){

    puts("");
    puts("Exchanging key with Alice...\n");
    puts("");

    int ret_val;
    unsigned char client_public_key[CRYPTO_PUBLICKEYBYTES];

    if(read(clientfd, client_public_key, CRYPTO_PUBLICKEYBYTES) == -1){
        perror("Error getting key from Bob\n");
        if(close(clientfd) == -1){
            perror("close clientfd"); /* server closed connection */
            exit(EXIT_FAILURE);
        } 
        exit(EXIT_FAILURE);
    }

    print_hex(stdout, "Bob's public Key: ", client_public_key, CRYPTO_PUBLICKEYBYTES);
    puts("");
    

    if ( (ret_val = crypto_kem_enc(cipher, sshared_key, client_public_key)) != 0) { /* ciphertext, hash, pk, only share ciphertext */
        printf("crypto_kem_enc returned <%d>\n", ret_val); /* ss is the shared key */
        exit(EXIT_FAILURE);
    }
    print_hex(stdout, "Cipher: ", cipher, CRYPTO_CIPHERTEXTBYTES);
    puts("");
    print_hex(stdout, "Symmetric Key: ", sshared_key, CRYPTO_BYTES);

    /* Send encrypted symmetric key to client */

    if(write(clientfd, cipher, CRYPTO_CIPHERTEXTBYTES) == -1){
        perror("Error sending encrypted symmetric key to Bob\n");
        if(close(clientfd) == -1){
            perror("close clientfd"); /* server closed connection */
            exit(EXIT_FAILURE);
        } 
        exit(EXIT_FAILURE);
    }

    puts("");
    printf("Key exchange is finished, you can now chat with Bob\n");
    puts("");

}

/*
    This function loop through all the socket address structure
    that matches hints, any (wildcard) ip address  from client  
    and a given port number server is listening on
    and uses the first one that works to connect to server
    and starts chatting with the server
*/

int find_ip_addr_and_listen(struct addrinfo * result){

    int listenfd;
    int optVal = 1;
    struct addrinfo * rp;
    for(rp = result; rp != NULL; rp = rp->ai_next){
        listenfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(listenfd == -1){
            continue;
        }

        /* This option is suggested by the book for TCP connection */
        if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) == -1){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if(bind(listenfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; //SUCCESS

        close(listenfd);

    }


    if(rp == NULL){
        puts("Could not bind any address");
        exit(EXIT_FAILURE);
    }

    /* listen on the established socket to any connecting client */
    if(listen(listenfd, BACKLOG) == -1 ){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); /* Free the addrinfo structure once we finish */
    return listenfd;
    
}