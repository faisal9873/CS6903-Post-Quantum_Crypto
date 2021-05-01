#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "server.h"
#include "print_hex.h"


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
    unsigned char       seed[48];
    unsigned char       cipher[crypto_ciphertextbytes], sshared_key[crypto_bytes];
    unsigned char       apublic_key[crypto_publickeybytes], aprivate_key[crypto_secretkeybytes];
    int                 ret_val;

    randombytes_init(seed, NULL, 256);
    printf("%llu %llu %llu %llu", crypto_publickeybytes, crypto_secretkeybytes, crypto_ciphertextbytes, crypto_bytes);
 
    if ( (ret_val = crypto_kem_keypair(apublic_key, aprivate_key)) != 0) {
        printf("crypto_kem_keypair returned <%d>\n", ret_val);
        exit(EXIT_FAILURE);
    }

    
    
    print_hex(stdout, "Public Key: ", apublic_key, crypto_publickeybytes);
    puts("");
    print_hex(stdout, "Private Key: ", aprivate_key, crypto_secretkeybytes);
    puts("");
    
    establish_connection(cipher, sshared_key);
    exit(EXIT_SUCCESS); 
}

