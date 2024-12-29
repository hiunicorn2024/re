#include <stdio.h>

main()
{
  int c, temp;

  c = EOF + 1;
  while (c != EOF && (c = getchar()) != EOF) {
    if (c == '\"') {
      putchar(c);
      while ((c = getchar()) != '\"' && c != EOF)
	putchar(c);
      if (c != EOF)
	putchar(c);
    } else if (c == '\'') {
      putchar(c);
      while ((c = getchar()) != '\'' && c != EOF)
	putchar(c);
      if (c != EOF)
	putchar(c);
    } else if (c == '/') {
      temp = c;
      if ((c = getchar()) == '*') {
	while ((c = getchar()) != '*' && c != EOF)
	  ;
	while (c != EOF && (c = getchar()) != '/')
	  while ((c = getchar()) != '*' && c != EOF)
	    ;
      }
      else
	putchar(c);
    } else
      putchar(c);
  }
}
