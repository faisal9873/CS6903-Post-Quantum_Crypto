#include "chat.h"

void print_smiley_face();
void handler(int signal);
char* encode(char* message);
char* decode(char* message);
char* find_token(char* msg, int len);

void handler(int signal){
    exit(EXIT_SUCCESS); /* Simply call exit to close any file descriptors */
}

/*
This function sets up so that the kernel uses 
the custom handler function for SIGINT and SIGQUIT

Input:
saptr : pointer to sigaction structure

*/

void establish_signal_handlers(struct sigaction* saptr){
    sigemptyset(&saptr->sa_mask);
    saptr->sa_flags = SA_RESTART;
    saptr->sa_handler = handler;
    if(sigaction(SIGINT, saptr, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGQUIT, saptr, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void print_smiley_face(){
    unsigned int actual_smiley_face = 0x1F60b;
    //utf-8 decoder
    char encode1 = (char) (0xf0 | (actual_smiley_face >> 18));
    char encode2 = (char) (0x80 | ((actual_smiley_face >> 12) & 0x3f));
    char encode3 = (char) (0x80 | ((actual_smiley_face >> 6) & 0x3f));
    char encode4 = (char) (0x80 | (actual_smiley_face & 0x3f));

    printf("%c%c%c%c ", encode1, encode2, encode3, encode4);
}

char* find_token(char* msg, int len){

    for(int i = 0; i+1 < len; ++i){
        if(strncmp(msg+i, "\\-", 2) == 0){
            if(i)
                msg[i-1] = '\0';
            return msg+i+3;
        }
    }
    return NULL;
}

/*
User press Ctrl-C or Ctrl-\ to exit the chat session

This function waits for input from either stdin, 
and write to the other end of the network
or from the other end of the network
and display to user on this end via stdout

*/

void chat(int clientfd, unsigned char* sshared_key){
    
    
    int ready, fd, num_read;
    
    int nfds = max(STDOUT_FILENO, clientfd) + 1;
    fd_set readfds, writefds;
    char* name = "me";
    char* client_name = "friend";
    unsigned char ciphertext[MESSAGE_LENGTH], plaintext[MESSAGE_LENGTH];
    char message[MESSAGE_LENGTH];
    int cipher_len, plaintext_len;
    char *iv;

    while(1){
        
        /*
            Block until either input from stdin
            or from server comes
        */

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(clientfd, &readfds);

        ready = select(nfds, &readfds, &writefds, NULL, NULL);

        if(ready == -1){
            if(errno == EINTR) // Ignore if interrupted by a signal
                continue;
            perror("select");
            exit(EXIT_FAILURE);
        }
        else{

            /* 
                Loop through all file descriptors
                to check for which io event triggers select
            */

            for(fd = 0; fd < nfds; fd++){

                /*
                    Only process triggered events
                */

                if(FD_ISSET(fd, &readfds)){
                    
                    /* 
                        Only set read events so we must read
                    */

                    num_read = read(fd, message, MESSAGE_LENGTH);

                    if(num_read == -1){

                        if(errno == EINTR) // Ignore if interrupted by a signal
                            continue;

                        perror("read");
                        exit(EXIT_FAILURE);

                    }
                    // message[num_read] = '\0';

                    /* encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext) */

                    if(fd == STDIN_FILENO){

                        /*
                            Encrypt message from user
                            and send to client
                        */
                        iv = find_token(message, num_read);
                        if(iv == NULL){
                            iv = "96458ehf8qbx7";
                        }
                        cipher_len = sym_encrypt( (unsigned char*) message, num_read, sshared_key, (unsigned char*) iv, ciphertext);
                        write(clientfd, ciphertext, cipher_len);
                        print_hex(stdout, "Encrypted message: ", ciphertext, cipher_len);
                    }
                    else {
                        
                        if(num_read == 0){

                            /*
                                Quit once server closes connection
                            */

                            printf("Closed connection with %s\n", client_name);
                            return;
                        }
                        else{

                            
                            /*
                                Decode and display message from server
                            */
                            iv = find_token(message, num_read);
                            if(iv == NULL){
                                iv = "96458ehf8qbx7";
                            }

                            plaintext_len = sym_decrypt((unsigned char*) message, num_read, sshared_key, (unsigned char*) iv, plaintext);
                            plaintext[plaintext_len] = '\0';
                            printf("%s ", client_name);
                            print_smiley_face();
                            printf(" %s", (char*) plaintext);
                            

                        }
                        

                    }
                    

                }
                
                
            }
        }

    }
    
}
