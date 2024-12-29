#include <stdio.h>
#include <ctype.h>

#define MAX 1000

int readline(char s[], int lim);
double atof(char a[]);

main()
{
  char a[MAX];

  readline(a, MAX);
  printf("%f\n", atof(a));
  return 0;
}

int readline(char s[], int lim) {
  int c, i = 0;

  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

double atof(char a[]) {
  int i, sign;
  double val, val2, power;

  val = val2 = 0;
  power = 1;
  for (i = 0; a[i] == ' ' || a[i] == '\t'; ++i)
    ;
  if (a[i] == '-') {
    ++i;
    sign = -1;
  }
  else if (a[i] == '+') {
    ++i;
    sign = 1;
  }
  else
    sign = 1;
  while (isdigit(a[i]))
    val = val * 10 + a[i++] - '0';
  if (a[i] == '.')
    for (++i; isdigit(a[i]); ++i) {
      val = val * 10 + a[i] - '0';
      power *= 10;
    }
  if (a[i] == 'e' || a[i] == 'E') {
    if (a[++i] == '-') {
      for(++i; isdigit(a[i]); ++i)
	val2 = val2 * 10 + a[i] - '0';
      while (val2--)
	power *= 10;
    }
    else {
      if (a[i] == '+') {
	++i;
	for(++i; isdigit(a[i]); ++i)
	  val2 = val2 * 10 + a[i] - '0';
      }
      else
	while (isdigit(a[i]))
	  val2 = val2 * 10 + a[i++] - '0';
      while (val2--)
	power /= 10;
    }
  }
  return sign * val / power;
}
