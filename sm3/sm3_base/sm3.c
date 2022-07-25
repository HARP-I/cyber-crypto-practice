/**
 * @file sm3.c
 * @author phanen
 * @brief Implementation of SM3
 *        - LOOK-UP TABLE
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "sm3.h"
#include "endian.h"
#include "func.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compress full blocks (from state)
static inline void
sm3_cf (uint32_t state[SM3_STATE_SZ_IN_WD], const uint8_t *input,
        size_t num_blks)
{
  uint32_t A, B, C, D, E, F, G, H;
  uint32_t W[68]; // gen W' online, reduce space
  uint32_t SS1, SS2, TT1, TT2;

  while (num_blks--)
    {
      // Message Expansion
      // W0 ~ W15
      for (int j = 0; j < 16; j++)
        W[j] = GETU32 (input + j * 4);
      // W16 ~ W67
      for (int j = 16; j < 68; j++)
        W[j] = P1 (W[j - 16] ^ W[j - 9] ^ ROL32 (W[j - 3], 15))
               ^ ROL32 (W[j - 13], 7) ^ W[j - 6];

      // Compress

      // init reg
      A = state[0];
      B = state[1];
      C = state[2];
      D = state[3];
      E = state[4];
      F = state[5];
      G = state[6];
      H = state[7];

      for (int j = 0; j < 16; j++)
        {
          SS1 = ROL32 ((ROL32 (A, 12) + E + K[j]), 7);
          SS2 = SS1 ^ ROL32 (A, 12);
          TT1 = FF00 (A, B, C) + D + SS2 + (W[j] ^ W[j + 4]); // online W'
          TT2 = GG00 (E, F, G) + H + SS1 + W[j];
          D = C;
          C = ROL32 (B, 9);
          B = A;
          A = TT1;
          H = G;
          G = ROL32 (F, 19);
          F = E;
          E = P0 (TT2);
        }

      for (int j = 16; j < 64; j++)
        {
          SS1 = ROL32 ((ROL32 (A, 12) + E + K[j]), 7); // online W'
          SS2 = SS1 ^ ROL32 (A, 12);
          TT1 = FF16 (A, B, C) + D + SS2 + (W[j] ^ W[j + 4]);
          TT2 = GG16 (E, F, G) + H + SS1 + W[j];
          D = C;
          C = ROL32 (B, 9);
          B = A;
          A = TT1;
          H = G;
          G = ROL32 (F, 19);
          F = E;
          E = P0 (TT2);
        }
      state[0] ^= A;
      state[1] ^= B;
      state[2] ^= C;
      state[3] ^= D;
      state[4] ^= E;
      state[5] ^= F;
      state[6] ^= G;
      state[7] ^= H;

      input += SM3_BLOCK_SZ_IN_BT;
    }
}

void
sm3_init (sm3_ctx_t *ctx)
{
  memset (ctx, 0, sizeof (sm3_ctx_t));
  // IV
  ctx->state[0] = 0x7380166F;
  ctx->state[1] = 0x4914B2B9;
  ctx->state[2] = 0x172442D7;
  ctx->state[3] = 0xDA8A0600;
  ctx->state[4] = 0xA96F30BC;
  ctx->state[5] = 0x163138AA;
  ctx->state[6] = 0xE38DEE4D;
  ctx->state[7] = 0xB0FB0E4E;

  //   ctx->cnt = 0;
  //   ctx->num = 0;
}

void
sm3_update (sm3_ctx_t *ctx, const uint8_t *input, size_t byte_len)
{
  size_t blocks;

  if (ctx->num) // if prev dataflow remain, handle remain bytes in last block
    {
      size_t left_len = SM3_BLOCK_SZ_IN_BT - ctx->num;
      if (byte_len < left_len) // not long enough to gen a full block
        {                      // append updated bytes
          memcpy (ctx->remain + ctx->num, input, byte_len);
          ctx->num += byte_len;
          return;
        }
      else // gen a full block
        {
          memcpy (ctx->remain + ctx->num, input, left_len);
          sm3_cf (ctx->state, ctx->remain, 1);
          ++ctx->cnt;
          input += left_len;
          byte_len -= left_len;
          // ctx->num will be reset later
        }
    }

  // num of full block
  blocks = byte_len / SM3_BLOCK_SZ_IN_BT;

  sm3_cf (ctx->state, input, blocks);
  ctx->cnt += blocks;
  input += SM3_BLOCK_SZ_IN_BT * blocks;
  byte_len -= SM3_BLOCK_SZ_IN_BT * blocks;

  ctx->num = byte_len;

  // store left_len bytes in current dataflow
  if (byte_len)
    memcpy (ctx->remain, input, byte_len);
}

void
sm3_finish (sm3_ctx_t *ctx, uint8_t *dgst)
{
  // append '10'
  ctx->remain[ctx->num] = 0x80;

  // append '0's
  if (ctx->num <= SM3_BLOCK_SZ_IN_BT - 9) // ctx->num + 9 <= 64
    {                                     // no new block
      memset (ctx->remain + ctx->num + 1, 0,
              SM3_BLOCK_SZ_IN_BT - ctx->num - 9);
    }
  else // ctx->num + 9 > 64
    {  // need new block
      memset (ctx->remain + ctx->num + 1, 0,
              SM3_BLOCK_SZ_IN_BT - ctx->num - 1);
      sm3_cf (ctx->state, ctx->remain, 1);
      // new block
      memset (ctx->remain, 0, SM3_BLOCK_SZ_IN_BT - 8);
    }

  // append (len)
  // PUTU32 (ctx->remain + 56, ctx->cnt >> 23);
  // PUTU32 (ctx->remain + 60, (ctx->cnt << 9) + (ctx->num << 3));
  PUTU64 (ctx->remain + 56, (ctx->cnt << 9) + (ctx->num << 3));

  sm3_cf (ctx->state, ctx->remain, 1);

  // copy to output
  for (int i = 0; i < 8; i++)
    PUTU32 (dgst + i * 4, ctx->state[i]);

  // free
  memset (ctx, 0, sizeof (sm3_ctx_t));
}

void
sm3_digest (const uint8_t *input, size_t byte_len, uint8_t *dgst)
{
  sm3_ctx_t ctx;
  sm3_init (&ctx);
  sm3_update (&ctx, input, byte_len);
  sm3_finish (&ctx, dgst);
}