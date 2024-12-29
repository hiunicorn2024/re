#include <stdio.h>

#define TABLEN 8

main()
{
  int c, i, pos, spnum;

  spnum = pos = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ')
      ++spnum;
    else if (c == '\n') {
      putchar(c);
      pos = 0;
    }
    else {
      while (spnum > TABLEN - pos % TABLEN) {
	spnum = spnum - (TABLEN - pos % TABLEN);
	putchar('\t');
	pos = 0;
      }
      while (spnum > 0) {
	putchar(' ');
	++pos;
	--spnum;
      }
      if (c == '\t')
	pos = 0;
      else
	++pos;
      putchar(c);
    }
  }
}
