#ifndef SM3_STD_H
#define SM3_STD_H

#include <inttypes.h>
#include <stdio.h>

#define SM3_BLOCK_SIZE_IN_BYTES 64
#define SM3_BLOCK_SIZE_IN_32 16

typedef struct
{
  uint32_t state[8];
  uint64_t bitcount;
  uint32_t buffer[16];
} sm3_context;

void sm3 (unsigned char *message, int message_length,
          unsigned char *digest /* 256-bits */);

#endif