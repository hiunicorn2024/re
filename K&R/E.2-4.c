#include <stdio.h>

#define MAXNUM 1000

int readline(char s[], int lim);
void squeeze(char s1[], char s2[]);

main()
{
  char a[MAXNUM], b[MAXNUM];

  readline(a, MAXNUM);
  readline(b, MAXNUM);
  squeeze(a, b);
  printf("%s", a);
}

int readline(char s[], int lim) {
  int c, i, j;

  i = j = 0;
  while ((c = getchar()) != EOF && c != '\n')
    if (j++ < lim - 1)
      s[i++] = c;
  if (c == '\n' && j < lim - 1)
    s[i++] = '\n';
  s[i] = '\0';
  return j;
}

void squeeze(char s1[], char s2[]) {
  int i, k, j;

  i = k = 0;
  while (s1[i] != '\0') {
    for (j = 0; s2[j] != s1[i] && s2[j] != '\0'; ++j)
      ;
    if (s2[j] == '\0')
      s1[k++] = s1[i];
    ++i;
  }
  s1[k] = '\0';
}
