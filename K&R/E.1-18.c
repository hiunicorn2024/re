#include <stdio.h>

#define BUFLIM 10000

main()
{
  int c, insp, isvalid, i;
  char sp[BUFLIM];

  insp = 1;
  i = isvalid = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\t') {
      if (insp == 0)
	insp = 1;
      if (i < BUFLIM - 1) {
	sp[i] = c;
	++i;
      }
    }
    else {
      if (insp == 1) {
	if (c != '\n') {
	  if (!isvalid)
	    isvalid = 1;
	  sp[i] = '\0';
	  printf("%s", sp);
	  putchar(c);
	  insp = 0;
	}
	else {
	  if (isvalid) {
	    putchar(c);
	    isvalid = 0;
	  }
	  insp = 1;
	}
	i = 0;
      }
      else
	putchar(c);
    }
  }
}
