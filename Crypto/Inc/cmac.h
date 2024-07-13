#ifndef CMAC_H
#define CMAC_H
#include "common.h"
#define const_Bsize 16

uint8_t* aes_cmac(const uint8_t* in, uint32_t length, uint8_t* out, uint8_t* key);
bool verify_mac(const uint8_t* in, uint32_t length, uint8_t* out, uint8_t* key);

void GenerateSubkey(uint8_t* key, uint8_t* K1, uint8_t* K2);

#endif // !CMAC_H
