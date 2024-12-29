#include <stdio.h>

#define LINELIM 200
#define FOLDLEN 20
#define TABLEN 8

int readline(char a[], int lim);

main()
{
  int i, j, pos, left;
  char s[LINELIM];

  while (readline(s, LINELIM) > FOLDLEN) {
    for (i = left = pos = 0; s[i] != '\0'; ) {
      if (s[i] == '\t')
	pos = pos + TABLEN - pos % TABLEN;
      else
	++pos;
      if (pos >= FOLDLEN) {
	j = i;
	if (s[i] == ' ' || s[i] == '\t') {
	  for (i = left; i < j; ++i)
	    putchar(s[i]);
	  putchar('\n');
	  left = i = j;
	}
	else {
	  while (i > left && s[i] != ' ' && s[i] != '\t')
	    --i;
	  if (i == left) {
	    while (i < j) {
	      putchar(s[i]);
	      ++i;
	    }
	    putchar('\n');
	    left = i = j;
	  }
	  else {
	    while (left <= i) {
	      putchar(s[left]);
	      ++left;
	    }
	    putchar('\n');
	    ++i;
	    left = i;
	  }
	}
	pos = 0;
      }
      else
	++i;
    }
    while (s[left] != '\0') {
      putchar(s[left]);
      ++left;
    }
  }
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
