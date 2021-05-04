#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "rng.h"
#include "api.h"
#include "server.h"
#include "print_hex.h"
#include "time_helper.h"



/*
Alice uses key generation to get her public and private key
Bob uses key generation to get her public and private key.

To establish a session, Alice encrypts a random message using AES 
in ECB mode using Bob's public key and sends Bob a ciphertext
while computing the hash of the random message as
the shared 256-bit key using SHA3-256 for symmetric
message exchanges.

Bob decrypts the ciphertext with his secret key 
to get the random message and compute the hash of 
this random message as the shared key.

The KEM is IND-CCA since Bob will not be able to 
decrypt Alice messages in the session using the shared
symmetric key if the original ciphertext was modified
by an attacker since the shared key was the output of
a hash function.


For each message, user sends with the form: 
    message \- random

random is used to initialize symmetric encryption 
of that message

ciphertext = (seq+1, IV, c=Enc(IV, key, m), Tag(seq+1, key, c)) = 32 + 16 + 256 + 32 bytes

*/


int main(){
    char                fn_req[32], fn_rsp[32];
    unsigned char       entropy_input[48], seed[48];
    unsigned char       cipher[crypto_ciphertextbytes], sshared_key[crypto_bytes];
    unsigned char       apublic_key[crypto_publickeybytes], aprivate_key[crypto_secretkeybytes];
    int                 ret_val;
    

    randombytes_init(entropy_input, NULL, 256);
    time_t t;
    srand((unsigned) time(&t));
    int rounds = rand() % 100;
    for(int i = 0; i < rounds; ++i){
        randombytes(seed, 48);
    }
    
    #ifdef OPEN_LOG
    FILE* measure_fd = fopen("./log/measure_stats", "a");
    if(measure_fd == NULL){
        printf("Failed to open file measure_stats for writing\n");
    }
    fprintf(measure_fd, "KEM: %s\n\n", PROG_NAME);
    fprintf(measure_fd, "Public Key Byte Size: %llu\n\n", crypto_publickeybytes);
    fprintf(measure_fd, "Private Key Byte Size: %llu\n\n", crypto_secretkeybytes);
    fprintf(measure_fd, "Message Byte Size: %llu\n\n", crypto_message_bytes);
    fprintf(measure_fd, "Pseudo random Byte Size: %llu\n\n", crypto_pseudorandom_secret_bytes);/* To be revised */
    fprintf(measure_fd, "Ciphertext Byte Size: %llu\n\n", crypto_ciphertextbytes);
    fprintf(measure_fd, "Symmetric Key Byte Size: %llu\n\n", crypto_bytes);
    

    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start,NULL);
    #endif
    
    randombytes_init(seed, NULL, 256);

    if ( (ret_val = crypto_kem_keypair(apublic_key, aprivate_key)) != 0) {
        printf("crypto_kem_keypair returned <%d>\n", ret_val);
        exit(EXIT_FAILURE);
    }
    
    #ifdef OPEN_LOG
    gettimeofday(&tv_end,NULL);
    fprintf(measure_fd, "Generate key pair takes %f seconds\n\n", tv_to_seconds(&tv_end) - tv_to_seconds(&tv_start) );

    if( fclose(measure_fd) == -1){
        fprintf(stderr, "Error closing measure_fd\n");
    }
    #endif
    
    
    print_hex(stdout, "Public Key: ", apublic_key, crypto_publickeybytes);
    puts("");
    print_hex(stdout, "Private Key: ", aprivate_key, crypto_secretkeybytes);
    puts("");
    
    establish_connection(cipher, sshared_key);
    exit(EXIT_SUCCESS); 
}

