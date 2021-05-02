#include "chat.h"

void print_smiley_face();
void handler(int signal);
char* encode(char* message);
char* decode(char* message);
int find_token(char* msg, int len, unsigned char* iv);

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

void combine_message(unsigned char* seq, unsigned char* iv, 
                    unsigned char* cipher, unsigned char* tag, 
                    unsigned char* message, unsigned char* cipher_len){
    
    memcpy(message, seq, SEQUENCE_BYTES);
    memcpy(message+SEQUENCE_BYTES, iv, IV_BYTES);
    memcpy(message+SEQUENCE_BYTES+IV_BYTES, cipher, *cipher_len);
    message[SEQUENCE_BYTES + IV_BYTES + CIPHER_BYTES] = *cipher_len;
    // memcpy(message+SEQUENCE_BYTES+IV_BYTES+CIPHER_BYTES, cipher_len, CIPHER_LENGTH_BYTES);
    memcpy(message+SEQUENCE_BYTES+IV_BYTES+CIPHER_BYTES+CIPHER_LENGTH_BYTES, tag, TAG_BYTES);

}

void get_message_components(unsigned char* seq, unsigned char* iv, 
                    unsigned char* cipher, unsigned char* tag, 
                    unsigned char* message, unsigned char* cipher_len){
    
    memcpy(seq, message, SEQUENCE_BYTES);
    memcpy(iv, message+SEQUENCE_BYTES, IV_BYTES);
    memcpy(cipher, message+SEQUENCE_BYTES+IV_BYTES, CIPHER_BYTES);
    *cipher_len = message[SEQUENCE_BYTES + IV_BYTES + CIPHER_BYTES];
    // memcpy(cipher_len, message+SEQUENCE_BYTES+IV_BYTES+CIPHER_BYTES, CIPHER_LENGTH_BYTES);
    memcpy(tag, message+SEQUENCE_BYTES+IV_BYTES+CIPHER_BYTES+CIPHER_LENGTH_BYTES, TAG_BYTES);

}

/*
User press Ctrl-C or Ctrl-\ to exit the chat session

This function waits for input from either stdin, 
and write to the other end of the network
or from the other end of the network
and display to user on this end via stdout

*/

void chat(int clientfd, unsigned char* sshared_key){
    
    
    int ready, fd, num_read, token_index;
    
    int nfds = max(STDOUT_FILENO, clientfd) + 1;
    fd_set readfds, writefds;
    char* name = "me";
    char* client_name = "friend";
    unsigned char ciphertext[CIPHER_BYTES], 
                plaintext[MESSAGE_BYTES], 
                seq[SEQUENCE_BYTES],
                seq_decrypt[SEQUENCE_BYTES], 
                seq_client[SEQUENCE_BYTES],
                iv[IV_BYTES],
                tag[TAG_BYTES],
                tag_decrypt[TAG_BYTES],
                seed[48];

    unsigned char message[TOTAL_CIPHERTEXT_BYTES];
    unsigned char cipher_len;
    int plaintext_len;
    randombytes_init(seed, NULL, 256);
    randombytes(seq, SEQUENCE_BYTES);
    // for(int i = 0; i < 4; ++i){
    //     printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(seq[i]));
    // }
    // puts("");
    printf("My Start Sequence: %ld\n", byte_to_bit(seq));
    int received_count = 0;

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

                    num_read = read(fd, message, TOTAL_CIPHERTEXT_BYTES);

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

                        randombytes(iv, IV_BYTES);

                        cipher_len = sym_encrypt( message, num_read , sshared_key, iv, ciphertext);

                        memcpy(message, seq, SEQUENCE_BYTES);
                        memcpy(message+SEQUENCE_BYTES, sshared_key, crypto_bytes);
                        memcpy(message+SEQUENCE_BYTES+crypto_bytes, ciphertext, cipher_len);

                        sha3_256(tag, message, SEQUENCE_BYTES+crypto_bytes + cipher_len);
                        combine_message(seq, iv, ciphertext, tag, message, &cipher_len);
                        write(clientfd, message, TOTAL_CIPHERTEXT_BYTES);

                        printf("Seq: %ld\n", byte_to_bit(seq));
                        // for(int i = 0; i < 4; ++i){
                        //     printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(seq[i]));
                        // }
                        // puts("");
                        print_hex(stdout, "IV: ", iv, IV_BYTES);
                        // print_hex(stdout, "Encrypted message: ", ciphertext, cipher_len);
                        print_hex(stdout, "Tag: ", tag, TAG_BYTES);
                        printf("Ciphertext length: %d\n", cipher_len);

                        cyclic_incr_byte_arr(seq);

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
                            if(num_read != TOTAL_CIPHERTEXT_BYTES){
                                printf("Received: %d bytes. Expected: %d bytes\n", num_read, TOTAL_CIPHERTEXT_BYTES);
                            }

                            get_message_components(seq_decrypt, iv, ciphertext, tag, message, &cipher_len);
                            

                            /*
                                Check sequence number for replay attack
                            */
                            if(received_count == 0){
                                memcpy(seq_client, seq_decrypt, SEQUENCE_BYTES);
                                printf("Start receiving sequence: %ld\n", byte_to_bit(seq_client));
                            }
                            else{
                                if(!compare_byte_arr(seq_client, seq_decrypt, SEQUENCE_BYTES)){
                                    printf("Sequence number not expected. Expected: %ld\n", byte_to_bit(seq_decrypt));
                                }
                            }
                            puts("");

                            printf("Seq: %ld\n", byte_to_bit(seq_decrypt));
                            print_hex(stdout, "IV: ", iv, IV_BYTES);
                            print_hex(stdout, "Encrypted message: ", ciphertext, cipher_len);
                            print_hex(stdout, "Tag: ", tag, TAG_BYTES);
                            printf("Ciphertext length: %d\n", cipher_len);

                            /*
                            Verify Tag
                            */

                            memcpy(message, seq_decrypt, SEQUENCE_BYTES);
                            memcpy(message+SEQUENCE_BYTES, sshared_key, crypto_bytes);
                            memcpy(message+SEQUENCE_BYTES+crypto_bytes, ciphertext, cipher_len);
                            sha3_256(tag_decrypt, message, SEQUENCE_BYTES+crypto_bytes + cipher_len);

                            if(!compare_byte_arr(tag, tag_decrypt, TAG_BYTES)){
                                printf("Verify failed.\n");
                                // print_hex(stdout, "Computed Tag: ", tag_decrypt, TAG_BYTES);
                                // print_hex(stdout, "Expected Tag: ", tag, TAG_BYTES);
                            }

                            /*
                                Decrypt
                            */

                            plaintext_len = sym_decrypt( ciphertext, cipher_len, sshared_key, iv, plaintext);
                            plaintext[plaintext_len] = '\0';
                            printf("%s ", client_name);
                            print_smiley_face();
                            printf(" %s", (char*) plaintext);
                            
                            received_count++;
                            cyclic_incr_byte_arr(seq_client);
                        }
                        

                    }

                    puts("");
                    

                }
                
                
            }
        }

    }
    
}
