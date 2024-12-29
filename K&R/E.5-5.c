#include <stdio.h>

char *strncpy_p(char *s, char *ct, int n);
char *strncat_p(char *s, char *ct, int n);
int strncmp_p(char *cs, char *ct, int n);

main()
{
  char a[20];
  char b[20] = "abcdefg";
  char c[20] = "0123";
  char d[20] = "abcdeaaa";

  printf("%s\n\n", strncpy_p(a, b, 4));
  printf("%s\n\n", strncat_p(c, b, 3));
  printf("%d\n", strncmp_p(b, d, 6));
  return 0;
}

char *strncpy_p(char *s, char *ct, int n)
{
  char *x = s;

  while (n--) {
    if (!*ct)
      break;
    *x++ = *ct++;
  }
  *x = '\0';
  return s;
}

char *strncat_p(char *s, char *ct, int n)
{
  char *x = s;

  while(*s)
    ++s;
  while (n--) {
    if (!*ct)
      break;
    *s++ = *ct++;
  }
  *s = '\0';
  return x;
}

int strncmp_p(char *cs, char *ct, int n)
{
  while(n-- && *cs == *ct && *ct && *cs) {
    ++ct;
    ++cs;
  }
  return *cs - *ct;
}
