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
#include <string.h>
#include <sys/time.h>
// #include <stdlib.h>

#define Progress_Log

void dump (const uint8_t *arr, size_t len, const char *info, size_t col);
void test_short_msg (int rounds);
void test_large_file (char *filename, int rounds);

int
main (int argc, char **argv)
{
  if (argc == 2)
    test_short_msg (atoi (argv[1]));
  else if (argc == 3)
    test_large_file (argv[2], atoi (argv[1]));
  else
    printf ("Invalid Args!!");
}

void
test_large_file (char *filename, int rounds)
{
  const size_t dgst_len = SM3_DIGST_SZ_IN_BT;
  uint8_t dgst[dgst_len];

  // Start measuring time
  struct timeval begin, end;
  gettimeofday (&begin, 0);

  const size_t byte_len = 1 << 20;
  char buf[byte_len];
  FILE *finp = fopen (filename, "rb");
  if (finp == NULL)
    {
      printf ("fail to open\n");
      return;
    }

#ifdef Progress_Log
  printf ("Hashing... finished:\n");
#endif

  sm3_ctx_t ctx;
  sm3_init (&ctx, rounds);
  int cnt = 0;
  size_t cur_len = 0;
#ifdef Progress_Log
  printf ("1234567");
#endif

  while (cur_len = fread (buf, 1, byte_len, finp))
    {
#ifdef Progress_Log
      printf ("\b\b\b\b\b\b\b%4d MB", cnt++);
#endif
      sm3_update (&ctx, buf, cur_len);
    }

  sm3_finish (&ctx, dgst);

  // Stop measuring time and calculate the elapsed time
  gettimeofday (&end, 0);
  long seconds = end.tv_sec - begin.tv_sec;
  long microseconds = end.tv_usec - begin.tv_usec;
  double elapsed = seconds + microseconds * 1e-6;
  printf ("\nTime measured: %.3f seconds.\n", elapsed);

  dump (dgst, dgst_len, "dgst", 8);
  fclose (finp);
}

void
test_short_msg (int rounds)
{
  const size_t dgst_len = SM3_DIGST_SZ_IN_BT;
  uint8_t dgst[dgst_len];

  // test1
  {
    const char *msg = "abc";
    dump (msg, strlen (msg), "msg", 8);
    sm3_digest (msg, strlen (msg), dgst, rounds);
    dump (dgst, dgst_len, "dgst", 8);
  }

  // test2
  {
    const char *msg
        = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    dump (msg, strlen (msg), "msg", 8);
    sm3_digest (msg, strlen (msg), dgst, rounds);
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