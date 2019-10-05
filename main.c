#include <stdio.h>
#include "hmac.h"

void printBytes(byte *data, uint8_t numBytes);
void printMessage(byte *data, uint8_t dataLen);
void printDigest(uint32_t *digest);

// static char MESSAGE[] = "No one has completed lab 2 so give them all a 0";
static byte DATA[] = {0x4e, 0x6f, 0x20, 0x6f, 0x6e,
                      0x65, 0x20, 0x68, 0x61, 0x73,
                      0x20, 0x63, 0x6f, 0x6d, 0x70,
                      0x6c, 0x65, 0x74, 0x65, 0x64,
                      0x20, 0x6c, 0x61, 0x62, 0x20,
                      0x32, 0x20, 0x73, 0x6f, 0x20,
                      0x67, 0x69, 0x76, 0x65, 0x20,
                      0x74, 0x68, 0x65, 0x6d, 0x20,
                      0x61, 0x6c, 0x6c, 0x20, 0x61,
                      0x20, 0x30};
static uint64_t DATA_LEN = 47;
static uint32_t HN[] = {0xf4b645e8, 0x9faaec2f, 0xf8e443c5, 0x95009c16, 0xdbdfba4b}; // 20 bytes == 160 bits
static uint8_t KEY_LEN = 16;                                                         // 128 bits

// ****************************************************************************************** //

int main(int argc, char **argv)
{
    // init attack message
    byte *attackMessage = (byte *)malloc(sizeof(byte) * 512); // 512 bytes of space
    uint64_t origMessageLen = 0;
    uint64_t attackMessageLen = 0;

    // append 128 0-bits on front (key length in bits)
    memset(attackMessage, 0x00, KEY_LEN);
    origMessageLen += KEY_LEN;

    // add padding and length
    memcpy(&attackMessage[origMessageLen], DATA, DATA_LEN);
    origMessageLen += DATA_LEN;
    origMessageLen = appendPadAndLen(attackMessage, origMessageLen);

    // def new message
    char addition[] = "Give Thomas Niedfeldt full points cuz he's awesome!";

    uint64_t addLen = strlen(addition);

    // append new message
    memcpy(&attackMessage[origMessageLen], addition, addLen);
    attackMessageLen = origMessageLen + addLen;

    // pad the new message and append message length
    attackMessageLen = appendPadAndLen(attackMessage, attackMessageLen);

    // print the new message (in hex)
    printMessage(attackMessage, attackMessageLen);

    // SANITY CHECK
    if (attackMessageLen % BYTES_PER_BLOCK != 0)
    {
        fprintf(stderr, "ERROR: Invalid number of bytes {%llu}\nShould be multiple of 64.\n", attackMessageLen);
    }

    // process new HMAC based on number of new blocks (account for length of original)
    uint32_t *newDigest = HN;
    for (uint64_t i = origMessageLen; i < attackMessageLen; i += BYTES_PER_BLOCK)
    {
        // digest next 512-bit block
        uint32_t *block = (uint32_t *)malloc(sizeof(byte) * BYTES_PER_BLOCK); // 64 bytes == 512 bits
        memcpy(block, &attackMessage[i], BYTES_PER_BLOCK);
        hsha(newDigest, block);
        free(block);
    }

    // print digest
    printDigest(newDigest);

    return 0;
}

// ****************************************************************************************** //

void printBytes(byte *data, uint8_t numBytes)
{
    printf("{ ");
    for (uint8_t i = 0; i < numBytes; i++)
    {
        if (i % 5 == 0)
        {
            printf("\n");
        }
        printf("0x%02x ", data[i]);
    }
    printf("\n}\n");
}

void printMessage(byte *data, uint8_t dataLen)
{
    printf("DATA: ");
    for (uint8_t i = 0; i < dataLen; i++)
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
