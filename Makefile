CC=/usr/bin/gcc
LDFLAGS=-lcrypto
CFLAGS=-I/usr/local/Cellar//openssl@1.1/1.1.1k/include -L/usr/local/Cellar//openssl@1.1/1.1.1k/lib/ -lssl -lcrypto
FLAGS = -std=c99 -Wall -O1 -D _POSIX_C_SOURCE=200809L 

NTRU_SOURCES = ntru/crypto_sort.c ntru/fips202.c ntru/kem.c ntru/owcpa.c ntru/pack3.c ntru/packq.c ntru/poly.c ntru/sample.c ntru/verify.c ntru/rng.c 
NTRU_HEADERS = ntru/api.h ntru/crypto_sort.h ntru/fips202.h ntru/kem.h ntru/poly.h ntru/owcpa.h ntru/params.h ntru/sample.h ntru/verify.h ntru/rng.h

APP_SOURCES = chat.c symmetric.c print_hex.c
APP_HEADERS = chat.h symmetric.h print_hex.h

chat_Alice: $(NTRU_HEADERS) $(NTRU_SOURCES) $(APP_HEADERS) $(APP_SOURCES) chat_Alice.c server.c server.h
	$(CC) -o $@ $(NTRU_SOURCES) $(APP_SOURCES) $(LDFLAGS) $(CFLAGS) chat_Alice.c server.c

chat_Bob: $(NTRU_HEADERS) $(NTRU_SOURCES) $(APP_HEADERS) $(APP_SOURCES) chat_Bob.c client.c client.h
	$(CC) -o $@ $(NTRU_SOURCES) $(APP_SOURCES) $(LDFLAGS) $(CFLAGS) chat_Bob.c client.c

.PHONY: clean

clean:
	-rm chat_Alice chat_Bob

