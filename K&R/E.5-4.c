#include <stdio.h>

int strend(char *s, char *t);

main()
{
  char a[] = "abcdefghij";
  char b[] = "ghij";
  printf("%d\n", strend(a, b));
  return 0;
}

int strend(char *s, char *t)
{
  char *m = t, *p = s;

  while (*s)
    ++s;
  while (*t)
    ++t;
  for (--s, --t; *s == *t; --s, --t)
    if (m == t && s >= p)
      return 1;
  return 0;
}
