#include "api.h"
#include "ntru/api.h"


unsigned long long crypto_secretkeybytes = CRYPTO_SECRETKEYBYTES;
unsigned long long crypto_publickeybytes = CRYPTO_PUBLICKEYBYTES;
unsigned long long crypto_ciphertextbytes = CRYPTO_CIPHERTEXTBYTES;
unsigned long long crypto_bytes = CRYPTO_BYTES;
unsigned long long crypto_message_bytes = NTRU_OWCPA_MSGBYTES;
unsigned long long crypto_pseudorandom_secret_bytes = NTRU_PRFKEYBYTES;
char* PROG_NAME = "NTRU";

