void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength);

int
randombytes(unsigned char *x,
            unsigned long long xlen);
