
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

#include <Windows.h>
#include <bcrypt.h>
#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include <unordered_map>
using namespace std;

void sm3_digest (const uint8_t *input, size_t byte_len, uint8_t *dgst,
                 int rounds);

const char *alpha_beta = "0123456789abcdef";

// to make hash a key --- we must make dgst comparable
// flaw: 1 byte was encoded into 2 bytes hex str...
inline string
bytes2str (uint8_t *bytes, size_t bytes_len)
{
  string ret;
  for (size_t i = 0; i < bytes_len; ++i)
    {
      ret.push_back (alpha_beta[bytes[i] & 0xf]);
      ret.push_back (alpha_beta[(bytes[i] >> 4) & 0xf]);
    }
  return ret;
}

void
rc ()
{
}
int
main ()
{
  random_device seed;
  ranlux48 engine (seed ());
  uniform_int_distribution<> distrib (0, 255);

  string a, b;

  // bytes
  constexpr size_t bytes_len = 7;
  constexpr size_t collision_len = 6;
  constexpr size_t hex_len = collision_len << 1;

  unordered_map<string, string> dgst2pmg;
  uint8_t pmg[bytes_len]{};
  uint8_t dgst[32]{};

  // Start measuring time
  struct timeval begin, end;
  gettimeofday (&begin, 0);

  cout << "searching..." << endl;

  srand (time (nullptr));
  while (true)
    {
      // rand preimage
      for (size_t i = 0; i < bytes_len; ++i)
        {
          pmg[i] = distrib (engine) & 0xff;
        }
      sm3_digest (pmg, bytes_len, dgst, 64);

      string cur_dgst_str = bytes2str (dgst, 32);
      string cur_pmg_str = bytes2str (pmg, bytes_len);

      cur_dgst_str = cur_dgst_str.substr (0, hex_len);
      // cout << cur_dgst_str << endl;
      if (dgst2pmg.count (cur_dgst_str))
        {

          auto pmg_str = dgst2pmg[cur_dgst_str];
          cout << dgst2pmg.size () << endl;
          if (pmg_str != cur_pmg_str)
            {
              // Sleep (21437);
              cout << "preimage1: " << pmg_str << endl;
              cout << "preimage2: " << cur_pmg_str << endl;
              cout << "hash: " << cur_dgst_str << " (" << collision_len
                   << " bytes)" << endl;
              break;
            }
          else
            continue;
        }
      dgst2pmg[cur_dgst_str] = cur_pmg_str;
    }
  // Stop measuring time and calculate the elapsed time
  gettimeofday (&end, 0);
  long seconds = end.tv_sec - begin.tv_sec;
  long microseconds = end.tv_usec - begin.tv_usec;
  double elapsed = seconds + microseconds * 1e-6;
  printf ("\nTime measured: %.3f seconds.\n", elapsed);
}
