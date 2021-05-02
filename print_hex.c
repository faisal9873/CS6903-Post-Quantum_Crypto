#include"print_hex.h"
#include <stdio.h>


void
print_hex(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}

unsigned long byte_to_bit(unsigned char* byte_arr){
	unsigned long ans = 0;
	for(int i = 3; i >= 0; --i){
		ans <<= 8;
		ans |= byte_arr[i];
		
	}
	return ans;
}

void bit_to_byte(unsigned long bit, unsigned char* byte_arr){
	
	for(int i = 0; i < 4; ++i){
		byte_arr[i] = bit & 0xff;
		bit >>= 8; 
	}

}

bool compare_byte_arr(unsigned char* byte_arr1, unsigned char* byte_arr2, int n){
	for(int i = 0; i < n; ++i){
		if(byte_arr1[i] != byte_arr2[i])
			return false;
	}
	return true;
}

void cyclic_incr_byte_arr(unsigned char* byte_arr){

	for(int i = 0; i < 4; ++i){
		if(byte_arr[i] == 0xff){
			byte_arr[i] = 0;
		}
		else {
			// byte_arr[i] = (byte_arr[i] << 1) & (~byte_arr[i]);
			// break;
			if((byte_arr[i] & 0x7f) == 0x7f){
				byte_arr[i] = (byte_arr[i] & 0x80) | 0x80;
			}
			else if((byte_arr[i] & 0x3f) == 0x3f){
				byte_arr[i] = (byte_arr[i] & 0xc0) | 0x40;
			}
			else if((byte_arr[i] & 0x1f) == 0x1f){
				byte_arr[i] = (byte_arr[i] & 0xe0) | 0x20;
			}
			else if((byte_arr[i] & 0x0f) == 0x0f){
				byte_arr[i] = (byte_arr[i] & 0xf0) | 0x10;
			}
			else if((byte_arr[i] & 0x07) == 0x07){
				byte_arr[i] = (byte_arr[i] & 0xf8) | 0x8;
			}
			else if((byte_arr[i] & 0x03) == 0x03){
				byte_arr[i] = (byte_arr[i] & 0xfc) | 0x4;
			}
			else if((byte_arr[i] & 0x01) == 0x01){
				byte_arr[i] = (byte_arr[i] & 0xfe) | 0x2;
			}
			else{//0
				byte_arr[i] = (byte_arr[i] & 0xff) | 0x1;
			}
			break;
		}
	}

}



/*
unsigned char byte_arr[4];

  

int main(){
	// printf("%ld", 4294967295 <<1);
	unsigned long num;
	printf("%d\n", 5);
	bit_to_byte(5, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 7);
	bit_to_byte(7, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 511);
	bit_to_byte(511, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%ld\n", 4294967295);
	bit_to_byte(4294967295, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");


	printf("%d\n", 0x5f);
	bit_to_byte(0x5f, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0xa);
	bit_to_byte(0xa, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0xaf);
	bit_to_byte(0xaf, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0x0ff);
	bit_to_byte(0x0ff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0x1ff);
	bit_to_byte(0x1ff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0x5ff);
	bit_to_byte(0x5ff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0xaff);
	bit_to_byte(0xaff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");


	printf("%d\n", 0x0fff);
	bit_to_byte(0x0fff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0x1fff);
	bit_to_byte(0x1fff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0x5fff);
	bit_to_byte(0x5fff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%d\n", 0xafff);
	bit_to_byte(0xafff, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	printf("%lu\n", 3918487953);
	bit_to_byte(3918487953, byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	cyclic_incr_byte_arr(byte_arr);
	for(int i = 0; i < 4; ++i){
		printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	}
	puts("");
	num = byte_to_bit(byte_arr);
	printf("%ld\n", num);
	puts("");

	// printf("%ld\n", 1003132915968);
	// bit_to_byte(1003132915968, byte_arr);
	// for(int i = 0; i < 4; ++i){
	// 	printf(""BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte_arr[i]));
	// }
	// puts("");
	// cyclic_incr_byte_arr(byte_arr);
	// num = byte_to_bit(byte_arr);
	// printf("%ld\n", num);
	// puts("");

	
}

*/

