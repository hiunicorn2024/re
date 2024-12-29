#include <stdio.h>

#define MAXNUM 1000

int readline(char s[], int lim);
int any(char a[], char b[]);

main()
{
  char a[MAXNUM], b[MAXNUM];

  readline(a, MAXNUM);
  readline(b, MAXNUM);
  printf("%d\n", any(a, b));
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

int any(char a[], char b[]) {
  int i, j;

  for (i = 0; a[i] != '\0'; ++i)
    for (j = 0; b[j] != '\0'; ++j)
      if (a[i] == b[j])
	return i;
  return -1;
}
