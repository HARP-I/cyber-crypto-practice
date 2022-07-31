// Put this in the same directory as the meow_hash_x64_aesni.h file from the
// Meow hash repo, then compile with: gcc -O3 -march=native tortoise_hare.c -o
// tortoise_hare

#include "sm3.h"
#include <iostream>
#include <stdint.h>
#include <stdio.h>
using namespace std;

uint64_t
f (uint64_t x)
{
  uint64_t pmg[4] = { x, x, x, x };
  uint64_t dgst[4] = { x, x, x, x };
  meow_u128 h = sm3 (pmg, 32, dgst);
  return *(uint64_t *)&h;
}

void
print_num (uint64_t x)
{
  uint8_t *byte_ptr = &x;
  for (int i = 0; i < 8; i++)
    printf ("%02x", byte_ptr[i]);
  printf ("\n");
}

int
main ()
{
  uint64_t tortoise = 0;
  uint64_t hare = 0;
  do
    {
      tortoise = f (tortoise);
      hare = f (f (hare));
    }
  while (tortoise != hare);
  printf ("Cycle detected! Hare location: ");

  print_num (hare);

  uint64_t x1, x2;
  tortoise = 0;
  while (tortoise != hare)
    {
      x1 = tortoise;
      x2 = hare;
      tortoise = f (tortoise);
      hare = f (hare);
    }

  printf ("Collision:\n");
  print_num (x1);
  print_num (x2);
}