#include "hmac.h"

uint64_t appendPadAndLen(byte *message, uint64_t messLen)
{
    // (messLen + paddingLen + keyLen) % 64 == 0
    uint64_t paddingLen = BYTES_PER_BLOCK - ((messLen + sizeof(uint64_t)) % BYTES_PER_BLOCK);

    // pad first byte with 0x80 and then 0x00 for all following padding
    if (paddingLen > 0)
    {
        message[messLen] = 0x80; // insert 1 and 0s
        if (paddingLen > 1)
        {
            memset(&message[messLen + 1], 0x00, paddingLen - 1);
        }
    }

    // add messLen on end
    uint64_t numBits = messLen * 8;
    byte *lenBytes = (byte *)&numBits; // convert long to byte-array
    for (uint8_t i = 0; i < 8; i++)
    {
        message[messLen + paddingLen + i] = lenBytes[7 - i];
    }

    return messLen + paddingLen + sizeof(uint64_t);
}

void hsha(uint32_t *IV, uint32_t *block)
{
    // prepare message schedule
    uint32_t *W = (uint32_t *)malloc(sizeof(uint32_t) * 80);
    for (uint8_t t = 0; t < 16; t++)
    {
        // flip the bytes
        uint8_t temp[4];
        byte *word = (byte *)&block[t];
        for (uint8_t i = 0; i < 4; i++)
        {
            temp[i] = word[3-i];
        }

        W[t] = *((uint32_t*)temp);
    }
    for (uint8_t t = 16; t < 80; t++)
    {
        uint32_t temp = W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16];
        W[t] = ROTL(temp, 1);
    }

    // init five working vars
    uint32_t a = IV[0];
    uint32_t b = IV[1];
    uint32_t c = IV[2];
    uint32_t d = IV[3];
    uint32_t e = IV[4];

    // for t=0-79, process
    for (uint8_t t = 0; t < 80; t++)
    {
        uint32_t temp = ROTL(a, 5) + f_t(t, b, c, d) + e + getK(t) + W[t];
        e = d;
        d = c;
        c = ROTL(b, 30);
        b = a;
        a = temp;
    }

    // compute hash value
    IV[0] = a + IV[0];
    IV[1] = b + IV[1];
    IV[2] = c + IV[2];
    IV[3] = d + IV[3];
    IV[4] = e + IV[4];

    // release memory
    free(W);
}

uint32_t ROTL(uint32_t x, uint8_t n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t f_t(uint8_t i, uint32_t x, uint32_t y, uint32_t z)
{
    if (i < 20)
    {
        return Ch(x, y, z);
    }
    else if (i < 40)
    {
        return Parity(x, y, z);
    }
    else if (i < 60)
    {
        return Maj(x, y, z);
    }
    else
    {
        return Parity(x, y, z);
    }
}
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z);
}
uint32_t Parity(uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ y ^ z;
}
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t getK(uint8_t i)
{
    if (i < 20)
    {
        return K[0];
    }
    else if (i < 40)
    {
        return K[1];
    }
    else if (i < 60)
    {
        return K[2];
    }
    else
    {
        return K[3];
    }
}
