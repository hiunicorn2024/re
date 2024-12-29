#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

int getfloat(double *pn);
int getch(void);
void ungetch(int);

main()
{
  double m;

  if (getfloat(&m))
    printf("%f\n", m);
  if (getfloat(&m))
    printf("%f\n", m);
  return 0;
}

/* getint:  get next float from input into *pn */
int getfloat(double *pn)
{
  int c, d, sign;
  double pow_10 = 1.0;
  
  while (isspace(c = getch()))   /* skip white space */
    ;
  if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
    ungetch(c);  /* it is not a number */
    return 0;
  }
  sign = (c == '-') ? -1 : 1;
  if (c == '+' || c == '-') {
    d = c;
    c = getch();
  }
  if (!isdigit(c)) {
    if (c != EOF)
      ungetch(c);
    ungetch(d);
    return '0';
  }
  for (*pn = 0.0; isdigit(c); c = getch())
    *pn = 10 * *pn + (c - '0');
  if (c == '.')
    c = getch();
  else
    goto s0;
  for (; isdigit(c); c = getch()) {
    *pn = 10 * *pn + (c - '0');
    pow_10 /= 10;
  }
 s0:
  *pn = *pn * pow_10 * sign;
  if (c != EOF)
    ungetch(c);
  return c;
}

char buf[BUFSIZE];    /* buffer for ungetch */
int bufp = 0;         /* next free position in buf */

int getch(void)  /* get a (possibly pushed-back) character */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}
