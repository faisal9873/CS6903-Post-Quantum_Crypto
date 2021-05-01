CC=/usr/bin/gcc
LDFLAGS=-lssl -lcrypto
CFLAGS=-I/usr/local/Cellar//openssl@1.1/1.1.1k/include -L/usr/local/Cellar//openssl@1.1/1.1.1k/lib/
FLAGS = -std=c99 -Wall -O1 -D _POSIX_C_SOURCE=200809L 
OUTPUTDIR = bin
INCLUDEDIR = includes
LIBDIR = lib
MCELIECEDIR = mceliece
MCELIECEFILE = libmceliece.a

NTRU_SOURCES = ntru/crypto_sort.c ntru/fips202.c ntru/kem.c ntru/owcpa.c ntru/pack3.c ntru/packq.c ntru/poly.c ntru/sample.c ntru/verify.c ntru/rng.c 
NTRU_HEADERS = ntru/api.h ntru/crypto_sort.h ntru/fips202.h ntru/kem.h ntru/poly.h ntru/owcpa.h ntru/params.h ntru/sample.h ntru/verify.h ntru/rng.h

APP_SOURCES = chat.c symmetric.c print_hex.c
APP_HEADERS = chat.h symmetric.h print_hex.h rng.h api.h

all: ntru_chat_Alice ntru_chat_Bob mceliece_chat_Alice mceliece_chat_Bob

${OUTPUTDIR}:
	@mkdir -p ${OUTPUTDIR}

ntru_chat_Alice: ${OUTPUTDIR} $(NTRU_HEADERS) $(NTRU_SOURCES) $(APP_HEADERS) $(APP_SOURCES) chat_Alice.c server.c server.h api_ntru.c
	$(CC) -o ${OUTPUTDIR}/$@ $(NTRU_SOURCES) $(APP_SOURCES) $(CFLAGS) chat_Alice.c server.c api_ntru.c ${LDFLAGS}

ntru_chat_Bob: ${OUTPUTDIR} $(NTRU_HEADERS) $(NTRU_SOURCES) $(APP_HEADERS) $(APP_SOURCES) chat_Bob.c client.c client.h api_ntru.c
	$(CC) -o ${OUTPUTDIR}/$@ $(NTRU_SOURCES) $(APP_SOURCES) $(CFLAGS) chat_Bob.c client.c api_ntru.c ${LDFLAGS}

${LIBDIR}/${MCELIECEDIR}/${MCELIECEFILE}:
	@mkdir -p ${LIBDIR}/${MCELIECEDIR}
	@make -C ${MCELIECEDIR}
	@cp ${MCELIECEDIR}/${LIBDIR}/${MCELIECEFILE} ${LIBDIR}/${MCELIECEDIR}/${MCELIECEFILE}

mceliece_deps:
	@mkdir -p ${INCLUDEDIR}/${MCELIECEDIR}
	@cp ${MCELIECEDIR}/*.h ${MCELIECEDIR}/**/*.h ${INCLUDEDIR}/${MCELIECEDIR}

mceliece_chat_Alice: ${OUTPUTDIR} ${LIBDIR}/${MCELIECEDIR}/${MCELIECEFILE} mceliece_deps ${APP_HEADERS} ${APP_SOURCES} chat_Alice.c server.c server.h api_mceliece.c
	$(CC) -o ${OUTPUTDIR}/$@ -I${INCLUDEDIR} -L${LIBDIR}/${MCELIECEDIR} ${CFLAGS} ${APP_SOURCES} chat_Alice.c server.c api_mceliece.c -lmceliece -lXKCP ${LDFLAGS}

mceliece_chat_Bob: ${OUTPUTDIR} ${LIBDIR}/${MCELIECEDIR}/${MCELIECEFILE} mceliece_deps ${APP_HEADERS} ${APP_SOURCES} chat_Bob.c client.c client.h api_mceliece.c
	${CC} -o ${OUTPUTDIR}/$@ -I${INCLUDEDIR} -L${LIBDIR}/${MCELIECEDIR} ${CFLAGS} ${APP_SOURCES} chat_Bob.c client.c api_mceliece.c -lmceliece -lXKCP ${LDFLAGS}

.PHONY: clean

clean:
	@rm -rf ${OUTPUTDIR}
	@rm -rf ${LIBDIR}
	@rm -rf ${INCLUDEDIR}

