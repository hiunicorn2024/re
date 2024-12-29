#include <stdio.h>

#define LIM 1000

main()
{
  int c, i, state;
  char a[LIM];

  i = 0;
  state = 1;
  while (state) {
    if (i < LIM - 1) {
      if ((c = getchar()) != '\n') {
	if (c != EOF) {
	  a[i] = c;
	  ++i;
	}
	else
	  state = 0;
      }
      else
	state = 0;
    }
    else
      state = 0;
  }
  if (c == '\n') {
    a[i] = '\n';
    ++i;
  }
  a[i] = '\0';
  printf("%s", a);
}
