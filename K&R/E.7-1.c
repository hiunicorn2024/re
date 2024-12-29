#include <stdio.h>
#include <ctype.h>

main(int argc, char **argv)
{
  int c;
  int state = 0;
  if (strcmp(*argv, "./a.out") == 0)
    state = 1;
  while ((c = getchar()) != EOF)
    putchar((state == 0 ? tolower : toupper)(c));
}
