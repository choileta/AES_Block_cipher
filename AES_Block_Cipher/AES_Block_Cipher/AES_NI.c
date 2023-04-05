#include <wmmintrin.h>
#include "type.h"
#include "AES.h"

inline __m128i AESNI_128_ASSIST(__m128i temp1, __m128i temp2) {
	__m128i temp3;
	temp2 = _mm_shuffle_epi32(temp2, 0xff);
	temp3 = _mm_slli_si128(temp1, 0x4);
	temp1 = _mm_xor_si128(temp1, temp3);
	temp3 = _mm_slli_si128(temp3, 0x4);
	temp1 = _mm_xor_si128(temp1, temp3);
	temp3 = _mm_slli_si128(temp3, 0x4);
	temp1 = _mm_xor_si128(temp1, temp3);
	temp1 = _mm_xor_si128(temp1, temp2);

	return temp1;
}

void AESNI_128_Key_Expansion(const unsigned char* userkey, unsigned char* key) {
	__m128i temp1, temp2;
	__m128i* Key_Schedule = (__m128i*)key;

	temp1 = _mm_loadu_si128((__m128i*)userkey);
	Key_Schedule[0] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x1);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[1] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x2);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[2] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x4);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[3] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x8);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[4] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x10);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[5] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x20);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[6] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x40);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[7] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x80);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[8] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x1b);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[9] = temp1;

	temp2 = _mm_aeskeygenassist_si128(temp1, 0x36);
	temp1 = AESNI_128_ASSIST(temp1, temp2);
	Key_Schedule[10] = temp1;
}

void AESNI_set_encrypt_key(const unsigned char* userKey, AESNI_KEY* key)
{
	AESNI_128_Key_Expansion(userKey, key->KEY);
	key->nr = 10;
}

void AESNI_core(const uint8_t* pt, uint8_t* roundkey, uint8_t* ct)
{
	__m128i tmp;
	//initial round
	tmp = _mm_loadu_si128(&((__m128i*)pt)[0]);
	tmp = _mm_xor_si128(tmp, ((__m128i*)roundkey)[0]);
	//Core round
	for (int j = 1; j < 10; j++) {
		tmp = _mm_aesenc_si128(tmp, ((__m128i*)roundkey)[j]);
	}
	//Final round
	tmp = _mm_aesenclast_si128(tmp, ((__m128i*)roundkey)[10]);
	_mm_storeu_si128(&((__m128i*)ct)[0], tmp);
}

void ENC_AES128_NI(const uint8_t* pt, const uint8_t* key, uint8_t* ct)
{
	AESNI_KEY KEY;
	AESNI_set_encrypt_key(key, &KEY);
	AESNI_core(pt, KEY.KEY, ct);
}