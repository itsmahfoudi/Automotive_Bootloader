#include "common.h"
#include "Utils.h"
//#define PASSORFAIL "\033[1;32mpassed\033[0m" : "\033[1;31mfailed\033[0m"

void print_bytes(uint8_t* buf, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

void block_xor(uint8_t* dst, uint8_t* a, uint8_t* b)
{
    for (int j = 0; j < 16; j++) {
        dst[j] = a[j] ^ b[j];
    }
}

void block_leftshift(uint8_t* dst, uint8_t* src)
{
    uint8_t ovf = 0x00;
    for (int i = 15; i >= 0; i--) {
        dst[i] = src[i] << 1;
        dst[i] |= ovf;
        ovf = (src[i] & 0x80) ? 1 : 0;
    }
}


