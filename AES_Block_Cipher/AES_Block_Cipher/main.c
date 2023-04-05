#include "AES.h"
#include <stdio.h>
#include <intrin.h>
__int64 cpucycles() {
	return __rdtsc();
}

int main()
{
	unsigned long long cycle1, cycle2;
	uint8_t pt[16] = { 0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34 };
	uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t ct[16] = { 0, };
	cycle1 = cpucycles();
	for (int i = 0; i < 100000; i++) {
		pt[i % 16] = pt[i % 16] ^ key[0] ^ (i % 16 + 1);
		key[i % 16] = pt[i % 15] ^ key[(i + 1) % 16] ^ (i % 16 + 4);
		ENC_AES128(pt, key, ct);
	}
	cycle2 = cpucycles();
	printf("8 bit-version RDTSC = %10lld\n", (cycle2 - cycle1) / 100000);

	cycle1 = cpucycles();
	for (int i = 0; i < 100000; i++) {
		pt[i % 16] = pt[i % 16] ^ key[0] ^ (i % 16 + 1);
		key[i % 16] = pt[i % 15] ^ key[(i + 1) % 16] ^ (i % 16 + 4);
		ENC_AES128_32bit(pt, key, ct);
	}
	cycle2 = cpucycles();
	printf("32 bit-version RDTSC = %10lld\n", (cycle2 - cycle1) / 100000);

	cycle1 = cpucycles();
	for (int i = 0; i < 100000; i++) {
		pt[i % 16] = pt[i % 16] ^ key[0] ^ (i % 16 + 1);
		key[i % 16] = pt[i % 15] ^ key[(i + 1) % 16] ^ (i % 16 + 4);
		ENC_AES128_NI(pt, key, ct);
	}
	cycle2 = cpucycles();
	printf("NI version RDTSC = %10lld\n", (cycle2 - cycle1) / 100000);


}