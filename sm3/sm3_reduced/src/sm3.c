/**
 * @file sm3.c
 * @author phanen
 * @brief Implementation of round-reduced SM3
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "sm3.h"
#include "endian.h"
#include "func.h"

#include <intrin.h> // SIMD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compress full blocks (from state)
static inline void
sm3_cf (sm3_ctx_t *ctx, const uint8_t *input, size_t num_blks)
{
  uint32_t A, B, C, D, E, F, G, H;
  uint32_t W[68]; // gen W' online, reduce space
  uint32_t SS1, SS2, TT1, TT2;

  // reducd to
  int rounds = ctx->rounds;

  while (num_blks--)
    {

      // init reg
      A = ctx->state[0];
      B = ctx->state[1];
      C = ctx->state[2];
      D = ctx->state[3];
      E = ctx->state[4];
      F = ctx->state[5];
      G = ctx->state[6];
      H = ctx->state[7];

      int j = 0;

      // Message Expansion
      __m128i X, Y, Z;
      __m128i M = _mm_setr_epi32 (0, 0, 0, 0xffffffff);

      for (; j < (rounds + 4 < 16 ? rounds + 4 : 16); j += 4)
        { // handle 4 words each time
          // For each word
          // reverse internal bytes' order before loading into U32
          // (default convert will interpret word in LE)

          X = _mm_shuffle_epi8 (
              _mm_loadu_si128 ((__m128i *)(input + (j << 2))), //
              _mm_setr_epi8 (3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13,
                             12) //
          );
          _mm_storeu_si128 ((__m128i *)(W + j), X);
        }

      // W[j] = P1 (W[j - 16] ^ W[j - 9] ^ ROL32 (W[j - 3], 15))
      //        ^ ROL32 (W[j - 13], 7) ^ W[j - 6];

      // reduce only half calcs
      for (; j < (rounds + 4 < 68 ? rounds + 4 : 68); j += 4)
        {
          /* X = (W[j - 3], W[j - 2], W[j - 1], 0) */
          X = _mm_andnot_si128 (M, _mm_loadu_si128 ((__m128i *)(W + j - 3)));
          X = _mm_rotl_epi32 (X, 15);

          // ^ W[j - 16] ^ W[j - 9]
          Y = _mm_xor_si128 (_mm_loadu_si128 ((__m128i *)(W + j - 16)),
                             _mm_loadu_si128 ((__m128i *)(W + j - 9)));
          X = _mm_xor_si128 (X, Y);

          // P1
          // shift then xor <=> xor then shift
          Y = _mm_rotl_epi32 (_mm_xor_si128 (_mm_rotl_epi32 (X, (23 - 15)), X),
                              15); // <<<23 ^ <<<15
          X = _mm_xor_si128 (X, Y);

          // ^ ROL32 (W[j - 13], 7) ^ W[j - 6]
          Y = _mm_rotl_epi32 (_mm_loadu_si128 ((__m128i *)(W + j - 13)), 7);
          X = _mm_xor_si128 (X, _mm_loadu_si128 ((__m128i *)(W + j - 6)));
          X = _mm_xor_si128 (X, Y);

          // done: W[j], W[j + 1], W[j + 2], 0
          // wait: W[j + 3]
          // W[j + 3] ^= P1(ROL32(W[j], 15))
          // 1. boardcast: W[j], W[j], W[j], W[j]
          // 2. mask: 0, 0, 0, W[j]
          Z = _mm_and_si128 (_mm_shuffle_epi32 (X, 0x0000000000), M);
          Y = _mm_rotl_epi32 (Z, 15);

          // Y --> 0, 15, 23
          // Z --> 15, 30, 6
          // Z --> 30, 15, 6
          //(diff)-> 15, 9, 6(Y) (30=15+9+6, 15=9+6, 6=6)

          Y = _mm_xor_si128 (Y, Z);                     // 15, 0
          Y = _mm_xor_si128 (_mm_rotl_epi32 (Y, 9), Z); // 24, 9, 0
          Y = _mm_rotl_epi32 (Y, 6);                    // 30, 15, 6

          X = _mm_xor_si128 (X, Y);
          _mm_storeu_si128 ((__m128i *)(W + j), X);
        }

      // 64 rounds' compress iteration
      j = 0;
      // Compress
      if (rounds >= 8)
        {
          R8 (A, B, C, D, E, F, G, H, 00);
        }
      if (rounds >= 16)
        {
          R8 (A, B, C, D, E, F, G, H, 00);
        }
      if (rounds >= 24)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }
      if (rounds >= 32)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }
      if (rounds >= 40)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }
      if (rounds >= 48)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }
      if (rounds >= 56)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }
      if (rounds >= 64)
        {
          R8 (A, B, C, D, E, F, G, H, 16);
        }

      ctx->state[0] ^= A;
      ctx->state[1] ^= B;
      ctx->state[2] ^= C;
      ctx->state[3] ^= D;
      ctx->state[4] ^= E;
      ctx->state[5] ^= F;
      ctx->state[6] ^= G;
      ctx->state[7] ^= H;

      input += SM3_BLOCK_SZ_IN_BT;
    }
}

void
sm3_init (sm3_ctx_t *ctx, int rounds)
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
  // rounds: 8x
  if (rounds <= 8)
    rounds = 8;
  if (rounds > 64)
    rounds = 64;
  ctx->rounds = (rounds >> 3) << 3;
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
          sm3_cf (ctx, ctx->remain, 1);
          ++ctx->cnt;
          input += left_len;
          byte_len -= left_len;
          // ctx->num will be reset later
        }
    }

  // num of full block
  blocks = byte_len / SM3_BLOCK_SZ_IN_BT;

  sm3_cf (ctx, input, blocks);
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
      sm3_cf (ctx, ctx->remain, 1);
      // new block
      memset (ctx->remain, 0, SM3_BLOCK_SZ_IN_BT - 8);
    }

  // append (len)
  // PUTU32 (ctx->remain + 56, ctx->cnt >> 23);
  // PUTU32 (ctx->remain + 60, (ctx->cnt << 9) + (ctx->num << 3));
  PUTU64 (ctx->remain + 56, (ctx->cnt << 9) + (ctx->num << 3));

  sm3_cf (ctx, ctx->remain, 1);

  // copy to output
  for (int i = 0; i < 8; i++)
    PUTU32 (dgst + i * 4, ctx->state[i]);

  // free
  memset (ctx, 0, sizeof (sm3_ctx_t));
}

void
sm3_digest (const uint8_t *input, size_t byte_len, uint8_t *dgst, int rounds)
{
  sm3_ctx_t ctx;
  sm3_init (&ctx, rounds);
  sm3_update (&ctx, input, byte_len);
  sm3_finish (&ctx, dgst);
}