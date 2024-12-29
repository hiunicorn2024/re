#include <stdio.h>
#include <ctype.h>

#define MAXNUM 1000

int htoi(char s[]);
int readline(char s[], int lim);

main()
{  
  char a[MAXNUM];

  readline(a, MAXNUM);
  printf("%d\n", htoi(a));
}

int htoi(char s[]) {
  int i, n;

  i = n = 0;
  if (s[i] == '0') {
    ++i;
    if (s[i] == 'x' || s[i] == 'X') {
      ++i;
      while (s[i] != '\0') {
	if (s[i] <= '9' && s[i] >= '0')
	  n = n * 16 + s[i] - '0';
	else if (s[i] <= 'F' && s[i] >= 'A')
	  n = n * 16 + s[i] - 'A' + 10;
	else if (s[i] <= 'f' && s[i] >= 'a')
	  n = n * 16 + s[i] - 'a' + 10;
	else
	  return n;
	++i;
      }
    }
  }
  return n;
}

int readline(char s[], int lim) {
  int c, i, j;

  i = j = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (j < lim - 1) {
      s[i] = c;
      ++i;
    }
    ++j;
  }
  if (c == '\n' && j < lim - 1) {
    s[i] = '\n';
    ++i;
  }
  s[i] = '\0';
  return j;
}
