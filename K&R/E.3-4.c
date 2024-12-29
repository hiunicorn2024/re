#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int readline(char s[], int lim);
void myitoa(int n, char s[]);
void reverse(char a[]);

main()
{
  char b[100];
  int a = 0, a_bk;

  while (--a < 0)
    a_bk = a;
  a = a_bk;
  printf("%d\n", a);
  myitoa(a, b);
  printf("%s\n", b);
}

int readline(char s[], int lim) {
  int c, i = 0;

  while (i + 1 < lim && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

void myitoa(int n, char s[]) {
  int i = 0, sign = n;

  do {
    s[i++] = abs(n % 10) + '0';
  } while (n /= 10);
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
