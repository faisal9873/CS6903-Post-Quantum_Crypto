#ifndef FIPS_MCELIECE_FIPS202_H
#define FIPS_MCELIECE_FIPS202_H

#include <libXKCP.a.headers/SimpleFIPS202.h> 

#define sha3_256(out,in,inlen) \
  SHA3_256(out,in,inlen)

#endif /* FIPS_MCELIECE_FIPS202_H */
