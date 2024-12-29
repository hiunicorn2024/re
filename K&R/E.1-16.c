#include <stdio.h>

#define MAXNUM 10

int readline(char a[], int lim);
void copy(char to[], char from[]);

main()
{
  int num, max;
  char s[MAXNUM], ss[MAXNUM];

  max = 0;
  while ((num = readline(s, MAXNUM)) > 0)
    if (num > max) {
      max = num;
      copy(ss, s);
    }
  printf("\nmaxlen:%d\n", max);
  printf("%s", ss);
}

int readline(char a[], int lim)
{
  int c, i, n;
  char s[lim];

  i = n = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (i + 1 < lim) {
      a[n] = c;
      ++n;
    }
    ++i;
  }
  if (c == '\n') {
    ++i;
    if (i + 1 < lim) {
      a[n] = '\n';
      ++n;
    }
  }
  a[n] = '\0';
  return i;
}

void copy(char to[], char from[])
{
  int i;

  i = 0;
  while ((to[i] = from[i]) != '\0')
    ++i;
}
