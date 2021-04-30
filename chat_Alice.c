#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ntru/rng.h"
#include "ntru/api.h"
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

*/


int main(){
    char                fn_req[32], fn_rsp[32];
    unsigned char       seed[48];
    unsigned char       cipher[CRYPTO_CIPHERTEXTBYTES], sshared_key[CRYPTO_BYTES];
    unsigned char       apublic_key[CRYPTO_PUBLICKEYBYTES], aprivate_key[CRYPTO_SECRETKEYBYTES];
    int                 ret_val;

    randombytes_init(seed, NULL, 256);

    if ( (ret_val = crypto_kem_keypair(apublic_key, aprivate_key)) != 0) {
        printf("crypto_kem_keypair returned <%d>\n", ret_val);
        exit(EXIT_FAILURE);
    }

    
    
    print_hex(stdout, "Public Key: ", apublic_key, CRYPTO_PUBLICKEYBYTES);
    puts("");
    print_hex(stdout, "Private Key: ", aprivate_key, CRYPTO_SECRETKEYBYTES);
    puts("");
    
    establish_connection(cipher, sshared_key);
    exit(EXIT_SUCCESS); 
}

