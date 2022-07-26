/**
 * @file main.c
 * @author phanen
 * @brief Run Test
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "sm3.h"
#include <stdio.h>
// #include <stdlib.h>
#include <string.h>

void dump (const uint8_t *arr, size_t len, const char *info, size_t col);
void test ();

int
main ()
{
  test ();
}

void
test ()
{
  {
    const char *msg = "abc";
    const size_t dgst_len = SM3_DIGST_SZ_IN_BT;
    uint8_t dgst[dgst_len];

    dump (msg, strlen (msg), "msg", 8);
    sm3_digest (msg, strlen (msg), dgst);
    dump (dgst, dgst_len, "dgst", 8);
  }

  {
    const char *msg
        = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    const size_t dgst_len = SM3_DIGST_SZ_IN_BT;
    uint8_t dgst[dgst_len];

    dump (msg, strlen (msg), "msg", 8);
    sm3_digest (msg, strlen (msg), dgst);
    dump (dgst, dgst_len, "dgst", 8);
  }
}

inline void
dump (const uint8_t *arr, size_t len, const char *info, size_t col)
{
  printf ("%s:\t(%u bytes)\n", info, len);
  for (size_t i = 0; i < len; ++i)
    {
      printf ("%02x ", arr[i]);
      if (i % col == col - 1)
        printf ("\n");
    }
  printf ("\n");
}