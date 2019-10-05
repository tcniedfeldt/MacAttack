#include <stdio.h>
#include "hmac.h"

static uint32_t IV[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};

void printBytes(uint8_t *data, uint64_t len);
void printDigest(uint32_t *digest);

// ****************************************************************************************** //

int main(int argc, char **argv)
{
    // sanity check on inputs
    if (argc != 2)
    {
        fprintf(stderr, "ERROR - NOT ENOUGH INPUTS\n");
        return 1;
    }

    // init message
    byte *message = (byte *)malloc(sizeof(byte) * 512); // 512 bytes of space
    uint64_t messageLen = strlen(argv[1]);              // number of character bytes
    memcpy(message, argv[1], messageLen);
    printBytes(message, messageLen);

    // pad message
    messageLen = appendPadAndLen(message, messageLen);
    printf("MESSAGE LEN: %llu\n", messageLen);
    printBytes(message, messageLen);

    // hash - assume only one block
    uint32_t *digest = IV;
    hsha(digest, (uint32_t *)message);
    printDigest(digest);
}

// ****************************************************************************************** //

void printBytes(uint8_t *data, uint64_t len)
{
    printf("BYTES (in hex): ");
    for (uint64_t i = 0; i < len; i++)
    {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void printDigest(uint32_t *digest)
{
    printf("DIGEST: ");
    for (uint8_t i = 0; i < 5; i++)
    {
        printf("%08x", digest[i]);
    }
    printf("\n");
}
