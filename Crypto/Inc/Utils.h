#ifndef UTILS_H
#define UTILS_H

#include "common.h"

void print_bytes(uint8_t* buf, const size_t len);
void block_xor(uint8_t* dst, uint8_t* a, uint8_t* b);
void block_leftshift(uint8_t* dst, uint8_t* src);


#endif // UTILS_H
