#include <stdio.h>

main()
{
  int inspace, c;

  inspace = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\t') {
      if (inspace == 0) {
	inspace = 1;
	putchar(' ');
      }
    }
    if (c != ' ' && c != '\t') {
      if (inspace == 1)
	inspace = 0;
      putchar(c);
    }
  }
}
