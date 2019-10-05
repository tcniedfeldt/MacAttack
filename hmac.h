#ifndef HMAC_H
#define HMAC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BYTES_PER_BLOCK 64

typedef uint8_t byte;

static uint32_t K[] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};

uint64_t appendPadAndLen(byte *message, uint64_t messLen);
void hsha(uint32_t *IV, uint32_t *block);

uint32_t ROTL(uint32_t x, uint8_t n);

uint32_t f_t(uint8_t i, uint32_t x, uint32_t y, uint32_t z);
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Parity(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

uint32_t getK(uint8_t i);

#endif
