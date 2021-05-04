# Quantum-Resistant Protocol for Two-Party Communication

# Usage

Alice and Bob binaries should be executed separately to allow them to talk to each other. To use Lattice based KEM NTRU and AES-256 in CBC mode with SHA3-256 as a tag scheme:
``` 
make
./bin/ntru_chat_Alice
./bin/ntru_chat_Bob
```

To use Code based KEM MCELIECE and AES-256 in CBC mode with SHA3-256 as a tag scheme:

```
make
./bin/mceliece_chat_Alice
./bin/mceliece_chat_Bob
```
