#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t table_h[256] = {
0x00, 0xdf, 0xbe, 0x61,  0x7c, 0xa3, 0xc2, 0x1d,  
0xd3, 0x0c, 0x6d, 0xb2,  0xaf, 0x70, 0x11, 0xce,  0x8d, 0x52, 0x33, 0xec,  0xf1, 0x2e, 0x4f, 0x90,  
0x5e, 0x81, 0xe0, 0x3f,  0x22, 0xfd, 0x9c, 0x43,  0x1a, 0xc5, 0xa4, 0x7b,  0x66, 0xb9, 0xd8, 0x07,  
0xc9, 0x16, 0x77, 0xa8,  0xb5, 0x6a, 0x0b, 0xd4,  0x97, 0x48, 0x29, 0xf6,  0xeb, 0x34, 0x55, 0x8a, 
0x44, 0x9b, 0xfa, 0x25,  0x38, 0xe7, 0x86, 0x59,  0x1f, 0xc0, 0xa1, 0x7e,  0x63, 0xbc, 0xdd, 0x02, 
0xcc, 0x13, 0x72, 0xad,  0xb0, 0x6f, 0x0e, 0xd1,  0x92, 0x4d, 0x2c, 0xf3,  0xee, 0x31, 0x50, 0x8f, 
0x41, 0x9e, 0xff, 0x20,  0x3d, 0xe2, 0x83, 0x5c,  0x05, 0xda, 0xbb, 0x64,  0x79, 0xa6, 0xc7, 0x18, 
0xd6, 0x09, 0x68, 0xb7,  0xaa, 0x75, 0x14, 0xcb,  0x88, 0x57, 0x36, 0xe9,  0xf4, 0x2b, 0x4a, 0x95, 
0x5b, 0x84, 0xe5, 0x3a,  0x27, 0xf8, 0x99, 0x46,  0x15, 0xca, 0xab, 0x74,  0x69, 0xb6, 0xd7, 0x08, 
0xc6, 0x19, 0x78, 0xa7,  0xba, 0x65, 0x04, 0xdb,  0x98, 0x47, 0x26, 0xf9,  0xe4, 0x3b, 0x5a, 0x85, 
0x4b, 0x94, 0xf5, 0x2a,  0x37, 0xe8, 0x89, 0x56,  0x0f, 0xd0, 0xb1, 0x6e,  0x73, 0xac, 0xcd, 0x12, 
0xdc, 0x03, 0x62, 0xbd,  0xa0, 0x7f, 0x1e, 0xc1,  0x82, 0x5d, 0x3c, 0xe3,  0xfe, 0x21, 0x40, 0x9f, 
0x51, 0x8e, 0xef, 0x30,  0x2d, 0xf2, 0x93, 0x4c,  0x0a, 0xd5, 0xb4, 0x6b,  0x76, 0xa9, 0xc8, 0x17, 
0xd9, 0x06, 0x67, 0xb8,  0xa5, 0x7a, 0x1b, 0xc4,  0x87, 0x58, 0x39, 0xe6,  0xfb, 0x24, 0x45, 0x9a, 
0x54, 0x8b, 0xea, 0x35,  0x28, 0xf7, 0x96, 0x49,  0x10, 0xcf, 0xae, 0x71,  0x6c, 0xb3, 0xd2, 0x0d, 
0xc3, 0x1c, 0x7d, 0xa2,  0xbf, 0x60, 0x01, 0xde,  0x9d, 0x42, 0x23, 0xfc,  0xe1, 0x3e, 0x5f, 0x80, 
0x4e, 0x91, 0xf0, 0x2f,  0x32, 0xed, 0x8c, 0x53
};
 
uint8_t table_l[256] = { 0x00, 0x2b, 0x57, 0x7c,  0xaf, 0x84, 0xf8, 0xd3,  
0xf6, 0xdd, 0xa1, 0x8a,  0x59, 0x72, 0x0e, 0x25,  0x45, 0x6e, 0x12, 0x39,  0xea, 0xc1, 0xbd, 0x96, 
0xb3, 0x98, 0xe4, 0xcf,  0x1c, 0x37, 0x4b, 0x60,  0x8b, 0xa0, 0xdc, 0xf7,  0x24, 0x0f, 0x73, 0x58, 
0x7d, 0x56, 0x2a, 0x01,  0xd2, 0xf9, 0x85, 0xae,  0xce, 0xe5, 0x99, 0xb2,  0x61, 0x4a, 0x36, 0x1d, 
0x38, 0x13, 0x6f, 0x44,  0x97, 0xbc, 0xc0, 0xeb,  0xbe, 0x95, 0xe9, 0xc2,  0x11, 0x3a, 0x46, 0x6d, 
0x48, 0x63, 0x1f, 0x34,  0xe7, 0xcc, 0xb0, 0x9b,  0xfb, 0xd0, 0xac, 0x87,  0x54, 0x7f, 0x03, 0x28, 
0x0d, 0x26, 0x5a, 0x71,  0xa2, 0x89, 0xf5, 0xde,  0x35, 0x1e, 0x62, 0x49,  0x9a, 0xb1, 0xcd, 0xe6, 
0xc3, 0xe8, 0x94, 0xbf,  0x6c, 0x47, 0x3b, 0x10,  0x70, 0x5b, 0x27, 0x0c,  0xdf, 0xf4, 0x88, 0xa3, 
0x86, 0xad, 0xd1, 0xfa,  0x29, 0x02, 0x7e, 0x55,  0xd4, 0xff, 0x83, 0xa8,  0x7b, 0x50, 0x2c, 0x07, 
0x22, 0x09, 0x75, 0x5e,  0x8d, 0xa6, 0xda, 0xf1,  0x91, 0xba, 0xc6, 0xed,  0x3e, 0x15, 0x69, 0x42, 
0x67, 0x4c, 0x30, 0x1b,  0xc8, 0xe3, 0x9f, 0xb4,  0x5f, 0x74, 0x08, 0x23,  0xf0, 0xdb, 0xa7, 0x8c, 
0xa9, 0x82, 0xfe, 0xd5,  0x06, 0x2d, 0x51, 0x7a,  0x1a, 0x31, 0x4d, 0x66,  0xb5, 0x9e, 0xe2, 0xc9, 
0xec, 0xc7, 0xbb, 0x90,  0x43, 0x68, 0x14, 0x3f,  0x6a, 0x41, 0x3d, 0x16,  0xc5, 0xee, 0x92, 0xb9, 
0x9c, 0xb7, 0xcb, 0xe0,  0x33, 0x18, 0x64, 0x4f,  0x2f, 0x04, 0x78, 0x53,  0x80, 0xab, 0xd7, 0xfc, 
0xd9, 0xf2, 0x8e, 0xa5,  0x76, 0x5d, 0x21, 0x0a,  0xe1, 0xca, 0xb6, 0x9d,  0x4e, 0x65, 0x19, 0x32, 
0x17, 0x3c, 0x40, 0x6b,  0xb8, 0x93, 0xef, 0xc4,  0xa4, 0x8f, 0xf3, 0xd8,  0x0b, 0x20, 0x5c, 0x77, 
0x52, 0x79, 0x05, 0x2e,  0xfd, 0xd6, 0xaa, 0x81
};

void crc_16(uint8_t in, uint8_t ptr[2])
{
	uint8_t r8, r10, r0, r11;

	r10 = ptr[1];
	r8 = ptr[0];
	r10 ^= in;
	r0 = table_h[r10];

	r0 ^= r8;
	r11 = table_l[r10];

	ptr[1] = r0;
	ptr[0] = r11;
}
int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <filename> <seek offset>\n", argv[0]);
		exit(1);
	}

	FILE *f = fopen(argv[1], "r");
	fseek(f, atoi(argv[2]), SEEK_SET);
	char buf[4096];
	uint8_t crc16[2] = {0, 0};
	int sz = 0;
	while ((sz = fread(buf, 1, 4096, f))) {
		int i;
		for (i = 0; i < sz; i ++) {
			crc_16(buf[i], crc16);
		}
	}

	fclose(f);
	printf("Computed CRC16 %#.2x %#.2x\n", crc16[1], crc16[0]);

	return 0;
}
