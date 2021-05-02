OUTPUTDIR = lib
OUTPUTNAME = libmceliece.a

all: ${OUTPUTDIR}/${OUTPUTNAME}

${OUTPUTDIR}/${OUTPUTNAME}: kat
	@mkdir -p ${OUTPUTDIR}
	@rm -f ${OUTPUTDIR}/${OUTPUTNAME}
	@ar -rc ${OUTPUTDIR}/${OUTPUTNAME} *.o

kat: Makefile nist/rng.c nist/rng.h randombytes.h benes.c bm.c controlbits.c decrypt.c encrypt.c gf.c operations.c pk_gen.c root.c sk_gen.c synd.c transpose.c util.c    
	@gcc -O3 -march=native -mtune=native -Wall -I. -Isubroutines -DKAT -DKATNUM=`cat KATNUM` "-DCRYPTO_NAMESPACE(x)=x" "-D_CRYPTO_NAMESPACE(x)=_##x" -c nist/rng.c benes.c bm.c controlbits.c decrypt.c encrypt.c gf.c operations.c pk_gen.c root.c sk_gen.c synd.c transpose.c util.c     -lXKCP -lcrypto -ldl 

.PHONY: clean
clean:
	@rm *.o
	@rm -r ${OUTPUTDIR}
