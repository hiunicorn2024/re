#include <stdio.h>

#define TABLEN 8

main()
{
  int c, i, pos;

  pos = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\t') {
      i = 8 - pos % TABLEN;
      while (i > 0) {
	putchar(' ');
	--i;
      }
      pos = 0;
    }
    else {
      if (c == '\n')
	pos = 0;
      else
	++pos;
      putchar(c);
    }
  }
}
