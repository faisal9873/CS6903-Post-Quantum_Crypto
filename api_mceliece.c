#include "api.h"


int crypto_kem_mceliece6688128_ref_keypair(unsigned char *, unsigned char *);
int crypto_kem_mceliece6688128_ref_enc(unsigned char *, unsigned char *, const unsigned char *);
int crypto_kem_mceliece6688128_ref_dec(unsigned char *, const unsigned char *, const unsigned char *);

int crypto_kem_keypair
(
       unsigned char *pk,
       unsigned char *sk 
)
{
    return crypto_kem_mceliece6688128_ref_keypair(pk, sk);
}

int crypto_kem_enc
(
    unsigned char *ct,
    unsigned char *ss,
    const unsigned char *pk
)
{
    return crypto_kem_mceliece6688128_ref_enc(ct, ss, pk);
}

int crypto_kem_dec
(
    unsigned char *ss,
    const unsigned char *ct,
    const unsigned char *sk
)
{
    return crypto_kem_mceliece6688128_ref_dec(ss, ct, sk);
}

#include "mceliece/crypto_kem.h"

unsigned long long crypto_secretkeybytes = crypto_kem_SECRETKEYBYTES;
unsigned long long crypto_publickeybytes = crypto_kem_PUBLICKEYBYTES;
unsigned long long crypto_ciphertextbytes = crypto_kem_CIPHERTEXTBYTES;
unsigned long long crypto_bytes = crypto_kem_BYTES;
unsigned long long crypto_message_bytes = 836; /* To be revised */
unsigned long long crypto_pseudorandom_secret_bytes = 0; /* To be revised */

char* PROG_NAME = "MCELIECE";