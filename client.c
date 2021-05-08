#include "client.h"
#include "print_hex.h"

#define BYTE_CHUNK 1024

void establish_connection(unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key){

    setbuf(stdout, NULL); // Set buf to null so we don't need to flush
                        // Use C stream and system call simultaneously
    struct sigaction sa;            //Handle SIGINT and SIGQUIT
                                    //to make sure exit is called
                                    //and all file descriptors are closed
    establish_signal_handlers(&sa);

    struct addrinfo hints;
    struct addrinfo * result;

    char* server_addr = LOCAL_ADDR;
    char* port = DEFAULT_PORT;

    /* 
    Create a structure to get all 
    possible ip address and port for 
    that matches the given
    server address and port number
    */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;//check when this is valid
                                //IPv4 or IPv6
    hints.ai_flags = AI_NUMERICSERV;
                                //Wildcard IP address; service name is numeric

    if(getaddrinfo(server_addr, port, &hints, &result) != 0)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);

    }
    
    connect_to_server(result, sshared_key, cipher, aprivate_key, apublic_key);

    exit(EXIT_SUCCESS); /* closes file descriptor*/
}

/*
    This function loop through all the socket address structure
    that matches hints, a given server ip address and port number
    and uses the first one that works to connect to server
    and starts chatting with the server
*/

void connect_to_server(struct addrinfo * result, unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key ){

    int serverfd;
    struct addrinfo *rp;
    /* Get one address that works and connect to server */

    for(rp = result; rp != NULL; rp = rp->ai_next){

        serverfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(serverfd == -1){
            continue;
        }

        if(connect(serverfd, rp->ai_addr, rp->ai_addrlen) != -1){
            break;
        }

        if( close(serverfd) == -1){
            perror("close serverfd");
            exit(EXIT_FAILURE);
        }

    }

    if(rp == NULL){
        
        puts("Could not connect to any address");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); /* Free the addrinfo structure once we finish */
    key_exchange(serverfd, sshared_key, cipher, aprivate_key, apublic_key);
    /* Starts chatting with server */
    chat(serverfd, sshared_key);

    if(close(serverfd) == -1){
        perror("close serverfd"); /* server closed connection */
        exit(EXIT_FAILURE);
    } 

}

void key_exchange(int serverfd, unsigned char* sshared_key, unsigned char* cipher, unsigned char* aprivate_key, unsigned char* apublic_key ){
    
    puts("");
    puts("Exchanging key with Bob...\n");
    puts("");

    int ret_val;
    /* Send public key to Alice */
    /* send this in chunks of 1KB */
    size_t write_result;
    size_t bytes_remaining = crypto_publickeybytes;
    while(bytes_remaining > BYTE_CHUNK){
        if((write_result = write(serverfd, apublic_key+crypto_publickeybytes-bytes_remaining, BYTE_CHUNK)) == -1){
            perror("Error sending public key to Alice\n");
            if(close(serverfd) == -1){
                perror("close serverfd");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        bytes_remaining -= write_result;
    }
    if(bytes_remaining > 0){
        if((write_result = write(serverfd, apublic_key+crypto_publickeybytes-bytes_remaining, bytes_remaining)) == -1){
            perror("Error sending public key to Alice\n");
            if(close(serverfd) == -1){
                perror("close serverfd");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
    }

/*    if((write_result = write(serverfd, apublic_key, crypto_publickeybytes)) == -1){
        perror("Error sending public key to Alice\n");
        if(close(serverfd) == -1){
        perror("close serverfd");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    printf("Bytes written: %zu\n", write_result);
*/
    /* Wait until receive the encrypted share key from Alice */

    if(read(serverfd, cipher, crypto_ciphertextbytes) == -1){
        perror("Error getting key from Alice\n");
        if(close(serverfd) == -1){
            perror("close serverfd"); /* server closed connection */
            exit(EXIT_FAILURE);
        } 
        exit(EXIT_FAILURE);
    }
    
    #ifdef OPEN_LOG
    FILE* measure_fd = fopen("./log/measure_stats", "a");
    if(measure_fd == NULL){
        printf("Failed to open file ./log/measure_stats for writing\n");
    }

    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start,NULL);
    #endif

    if ( (ret_val = crypto_kem_dec(sshared_key, cipher, aprivate_key)) != 0) {
        printf("crypto_kem_dec returned <%d>\n", ret_val); 
        exit(EXIT_FAILURE);
    }

    #ifdef OPEN_LOG
    gettimeofday(&tv_end,NULL);
    fprintf(measure_fd, "Decryption takes %f seconds\n\n", tv_to_seconds(&tv_end) - tv_to_seconds(&tv_start) );

    if( fclose(measure_fd) == -1){
        fprintf(stderr, "Error closing measure_fd\n");
    }
    #endif

    print_hex(stdout, "Symmetric Key: ", sshared_key, crypto_bytes);

    puts("");
    printf("Key exchange is finished, you can now chat with Alice (max message length: 238)\n");
    puts("");

}
