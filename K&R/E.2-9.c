#include <stdio.h>

int bitcount(unsigned x);

main()
{
  unsigned a = 0xBB1;

  printf("1101 1011 0001\n");
  printf("bitcount:%d\n", bitcount(a));
}

int bitcount(unsigned x)
{
  int i;

  for (i = 0; x != 0; ++i)
    x &= (x - 1);
  return i;
}
