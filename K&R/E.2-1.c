#include <stdio.h>
 
main()
{
  char c0, c1;
  signed char sc0, sc1;
  unsigned char uc0, uc1;
  int d0, d1;
  unsigned int u0, u1;
  short s0, s1;
  unsigned short su0, su1;
  long l0, l1;
  unsigned long lu0, lu1;

  c0 = c1 = 0;
  while ((c1 = c0 - 1) < c0)
    --c0;
  printf("char\nmin:\t %ld\nmax:\t %lu\n", c0, c1);
  c0 = c1 = 0;
  while ((c1 = c0 - 1) < c0)
    --c0;
  printf("signed char\nmin:\t %ld\nmax:\t %lu\n", c0, c1);
  uc0 = uc1 = 0;
  while ((uc1 = uc0 - 1) < uc0)
    --uc0;
  printf("unsigned char\nmin:\t %lu\nmax:\t %lu\n", uc0, uc1);
  d0 = d1 = 0;
  while ((d1 = d0 - 1) < d0)
    --d0;
  printf("int\nmin:\t %ld\nmax:\t %lu\n", d0, d1);
  u0 = u1 = 0;
  while ((u1 = u0 - 1) < u0)
    --u0;
  printf("unsigned int\nmin:\t %lu\nmax:\t %lu\n", u0, u1);
  s0 = s1 = 0;
  while ((s1 = s0 - 1) < s0)
    --s0;
  printf("short\nmin:\t %ld\nmax:\t %lu\n", s0, s1);
  su0 = su1 = 0;
  while ((su1 = su0 - 1) < su0)
    --su0;
  printf("unsigned short\nmin:\t %lu\nmax:\t %lu\n", su0, su1);
  l0 = l1 = 0;
  while ((l1 = l0 - 1) < l0)
    --l0;
  printf("long\nmin:\t %ld\nmax:\t %lu\n", l0, l1);
  lu0 = lu1 = 0;
  while ((lu1 = lu0 - 1) < lu0)
    --lu0;
  printf("unsigned long\nmin:\t %lu\nmax:\t %lu\n", lu0, lu1);
}
