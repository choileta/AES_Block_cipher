#pragma once
#include "type.h"

typedef struct AES {
	uint8_t  KEY[16 * 11];
	uint32_t rounds;
}AES_KEY;

void ENC_AES128(const uint8_t* pt, const uint8_t* key, uint8_t* ct);

#define ROTL(x, n)			(((x) << (n)) | ((x) >> (32 - (n))))
#define ENDIAN_CHANGE(X)	((ROTL((X),  8) & 0x00ff00ff) | (ROTL((X), 24) & 0xff00ff00))
typedef struct AES_32 {
	uint32_t KEY[4 * 11];
	uint32_t rounds;
}AES_KEY32;
void ENC_AES128_32bit(const uint8_t* pt, const uint8_t* key, uint8_t* ct);


#if !defined (ALIGN16)
#	if defined(__GNUC__)
#		define ALIGN16	__attribute__((aligned(16)))
#else
#	define ALIGN16	__declspec(align(16))
#endif
#endif

typedef struct KEY_SCHEDULE {
	ALIGN16 unsigned char KEY[16 * 15];
	unsigned int nr;
} AESNI_KEY;
void ENC_AES128_NI(const uint8_t* pt, const uint8_t* key, uint8_t* ct);