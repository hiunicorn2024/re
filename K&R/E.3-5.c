#include <stdio.h>

void itob(int n, char s[], int b);
void reverse(char a[]);

main()
{
  char s[100];

  itob(65535, s, 8);
  printf("%s\n", s);
}

void itob(int n, char s[], int b) {
  int sign, i = 0;
  long int m;

  if (b > 16 || b < 2) {
    s[0] = '\0';
    return;
  }
  m = n;
  if ((sign = n) < 0)
    m = -m;
  do {
    if (b <= 10)
      s[i++] = m % b + '0';
    else
      s[i++] = m % b - 10 + 'a';
  } while ((m /= b) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
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
