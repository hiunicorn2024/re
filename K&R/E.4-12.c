#include <stdio.h>

void itoa(char a[], int m);

main()
{
  int a = -100, b = 4321321;
  char s[20];

  itoa(s, a);
  printf("%s\n", s);
  itoa(s, b);
  printf("%s\n", s);
  itoa(s, b);
  printf("%s\n", s);
  return 0;
}

void itoa(char a[], int m)
{
  static int i = 0, k = 1;

  if (k == 0) {
    i = 0;
    k = 1;
  }
  if (m < 0) {
    a[i++] = '-';
    m = -m;
  }
  if (m / 10)
    itoa(a, m / 10);
  else
    k = 0;
  a[i++] = m % 10 + '0';
  a[i] = '\0';
}
