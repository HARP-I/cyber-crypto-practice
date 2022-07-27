/**
 * @file sm3.h
 * @author phanen
 * @brief API of SM3
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include <stdint.h>

#define SM3_DIGST_SZ_IN_BT 32 // 256
#define SM3_BLOCK_SZ_IN_BT 64 // 512 = 64 bytes = 16 words
// sizeof(middle state) == sizeof(final digest)
#define SM3_STATE_SZ_IN_WD 8 // 256 = 32 bytes = 8 words

typedef struct sm3_ctx
{
  // counter of message bit length
  uint64_t cnt;
  // num of remain bytes in remain incomplete block
  size_t num;
  // middle state of iteration
  uint32_t state[SM3_STATE_SZ_IN_WD];
  // store remain incomplete block in a flow
  uint8_t remain[SM3_BLOCK_SZ_IN_BT];
  uint8_t rounds;
} sm3_ctx_t;

/**
 * @brief initialize the context
 *
 * @param ctx
 */
void sm3_init (sm3_ctx_t *ctx, int rounds);

/**
 * @brief update middle state
 *        compress a continuous dataflow
 * @param ctx
 * @param input cont inuous dataflow in address
 * @param byte_len byte length of input
 */
void sm3_update (sm3_ctx_t *ctx, const uint8_t *input, size_t byte_len);

/**
 * @brief finish the last block and write final output
 *       (pad, then compress)
 * @param ctx
 * @param dgst return final digest
 */
void sm3_finish (sm3_ctx_t *ctx, uint8_t *dgst);

/**
 * @brief simple hash (single short message)
 *        invoke other func
 * @param input
 * @param byte_len
 * @param dgst return
 */
void sm3_digest (const uint8_t *input, size_t byte_len, uint8_t *dgst,
                 int rounds);

// legacy ==================================
// free the context
// empty func, no need to free (no heap alloc)
// void sm3_free (sm3_ctx_t *ctx);

// reset the context for new hash
// empty func, no need to reset
// void sm3_reset (sm3_ctx_t *ctx);