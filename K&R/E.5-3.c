#include <stdio.h>

void strcat_p(char *s, char *t);

main()
{
  char a[100] = "abcdne";
  char b[100] = "fghij";

  strcat_p(a, b);
  printf("%s\n", a);
  return 0;
}

void strcat_p(char *s, char *t)
{
  while (*s)
    ++s;
  while (*s++ = *t++)
    ;
}
