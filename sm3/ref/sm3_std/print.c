#include "print.h"
#include <stdio.h>

void
print_bytes (unsigned *buf, int n)
{
  uint8_t *ptr = (uint8_t *)buf;

  for (int i = 0; i <= n / 4; i++)
    {
      if (i > 0 && i % 8 == 0)
        {
          debug_print ("\n");
        }
      for (int j = 1; j <= 4; j++)
        {
          if ((i * 4 + 4 - j) < n)
            {
              debug_print ("%.2X", ptr[(i * 4) + 4 - j]);
            }
        }
      debug_print (" ");
    }
  debug_print ("\n");
}

void
print_block (unsigned *buf, int n)
{
  print_bytes (buf, n * 4);
}

void
print_af (int i, uint32_t A, uint32_t B, uint32_t C, uint32_t D, uint32_t E,
          uint32_t F, uint32_t G, uint32_t H)
{
  if (i % 10 == 0)
    {
      debug_print ("\n");
      debug_print ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                   " j = %2d "
                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",
                   i);
    }
  debug_print ("%.8X ", (unsigned)A);
  debug_print ("%.8X ", (unsigned)B);
  debug_print ("%.8X ", (unsigned)C);
  debug_print ("%.8X ", (unsigned)D);
  debug_print ("%.8X ", (unsigned)E);
  debug_print ("%.8X ", (unsigned)F);
  debug_print ("%.8X ", (unsigned)G);
  debug_print ("%.8X", (unsigned)H);
  debug_print ("\n");
}

void
print_hash (unsigned *buf)
{
  print_block (buf, 8);
}