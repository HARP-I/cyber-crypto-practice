
/**
 * @file sm3_birthday_attack.cc
 * @author phanen
 * @brief
 * @date 2022-07-27
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "endian.h"
#include "sm3.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
// #include <sm3.h>
// #include <unordered_map>
using namespace std;

int
main ()
{
  unordered_map<uint8_t *, uint8_t *> dgst2pmg;

  size_t byte_len = 32;
  int rounds = 32;

  uint8_t slow_buf[32];
  uint8_t fast_buf[32];

  srand (time (nullptr));

  // set a rand start point
  uint8_t *pmg_buf = malloc (byte_len);
  for (uint8_t *byte_ptr = pmg_buf; byte_ptr != buf + byte_len; ++byte_ptr)
    byte_ptr = (rand ()) & 0xff;

  while (sm3_)
    { // slow

      sm3_digest (pmg_buf, byte_len, slow_buf, 26);
      sm3_digest (dgst, byte_len, dgst_buf);

      // fast
      sm3_digest (pmg_buf, byte_len, dgst_buf);
      sm3_digest (dgst, byte_len, dgst_buf);

      uint8_t *pmg_new = new uint8_t[byte_len];
      memcpy (pmg_new, pmg_buf, byte_len);

      uint8_t *dgst_new = new uint8_t[32];
      memcpy (dgst_new, dgst_buf, 32);
    }

  free (buf);
}

//  Iteration instead of RNG each time

// for (uint8_t *byte_ptr = pmg_buf; byte_ptr != buf + byte_len; ++byte_ptr)
//   {

//     for (uint8_t j = 0; j < 256; j++)
//       {
//         *byte_ptr = j;

//         sm3_digest (pmg_buf, byte_len, dgst_buf);
//         sm3_digest (dgst, byte_len, dgst_buf);

//         uint8_t *pmg_new = new uint8_t[byte_len];
//         memcpy (pmg_new, pmg_buf, byte_len);

//         uint8_t *dgst_new = new uint8_t[32];
//         memcpy (dgst_new, dgst_buf, 32);

//         dgst2pmg.insert (make_pair (new uint8_t ()))
//       }
//   }