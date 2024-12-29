#include <stdio.h>

#define MAXNUM 1000

int readline(char s[], int lim);
void reverse(char m[]);

main()
{
  char ss[MAXNUM];

  while (readline(ss, MAXNUM) > 0) {
    reverse(ss);
    printf("%s", ss);
  }
}

int readline(char a[], int lim)
{
  int c, i, n;
  char s[lim];

  i = n = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (i < lim - 1) {
      a[n] = c;
      ++n;
    }
    ++i;
  }
  if (c == '\n') {
    ++i;
    if (i < lim - 1) {
      a[n] = '\n';
      ++n;
    }
  }
  a[n] = '\0';
  return i;
}

void reverse(char m[]) {
  int i, n, l;
  char x;

  for (i = 0; m[i] != '\0'; ++i)
    ;
  l = i - 1;
  n = i / 2;
  for (i = 0; i < n; ++i) {
    x = m[i];
    m[i] = m[l - i];
    m[l - i] = x;
  }
}
