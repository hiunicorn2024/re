#include <stdio.h>

#define MAX 1000

int readline(char s[], int lim);
int strindex(char s[], char t[]);

main()
{
  char a[MAX], b[MAX];
  int c;

  if (readline(a, MAX) > 0 && readline(b, MAX) > 0)
    if (c = strindex(a, b))
      printf("%d\n", c);
  return 0;
}

int readline(char s[], int lim) {
  int c, i = 0;

  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

int strindex(char s[], char t[]) {
  int i, j, k, lastp = -1;

  for (i = 0; s[i] != '\0'; ++i) {
    for (j = i, k = 0; s[k] != '\0' && s[j] == t[k]; ++k, ++j)
      ;
    if (k > 0 && t[k] == '\0')
      lastp = i;
  }
  return lastp;
}
