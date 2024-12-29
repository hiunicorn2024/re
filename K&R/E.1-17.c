#include <stdio.h>

#define MINLIM 11

int printline(char s[], int lim);

main()
{
  char buff[MINLIM];

  while (printline(buff, MINLIM) > 0)
    ;
}

int printline(char s[], int lim) {
  int i, j, c;

  i = j = 0;
  while((c = getchar()) != EOF && c != '\n') {
    ++i;
    if (i < lim) {
      s[j] = c;
      ++j;
    }
    else {
      if (i == lim) {
	s[j] = c;
	++j;
	s[j] = '\0';
	printf("%s", s);
      }
      else
	putchar(c);
    }
  }
  if (c == '\n') {
    ++i;
    if (i == lim) {
      s[j] = '\0';
      printf("%s", s);
      putchar(c);
      return i;
    }
    else if (i > lim) {
      s[j] = '\0';
      putchar(c);
      return i;
    }
    s[j] = c;
    ++j;
  }
  s[j] = '\0';
  return i;
}
