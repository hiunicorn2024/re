#include <stdio.h>

void itoa(int n, char s[], int width);
void reverse(char a[]);

main()
{
  char s[100];

  itoa(65535, s, 8);
  printf("%s\n", s);
}

void itoa(int n, char s[], int width) {
  int i, sign, k;

  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
  if (width > i) {
    for (k = 0; k < i + 1; ++k)
      s[width - k] = s[i - k];
    for (k = 0; k < width - i; ++k)
      s[k] = ' ';
  }
}

void reverse(char a[]) {
  int i, n = 0;
  char temp;

  while (a[n] != '\0')
    ++n;
  for (i = 0; i < n / 2; ++i) {
    temp = a[i];
    a[i] = a[n - 1 - i];
    a[n - 1 - i] = temp;
  }
}
