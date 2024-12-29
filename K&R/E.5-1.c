#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

char buf[BUFSIZE];		/* buffer for ungetch */
int bufp = 0;			/* next free position in buf */

int getint(int *pn);
int getch(void);
void ungetch(int);

main()
{
  int m, state;

  if ((state = getint(&m)) > 0 && state != '0')
    printf("%d\n", m);
  if (state == '0')
    printf("\n**%c**\n", getch());
}

/* getint:  get next integer from input into *pn */
int getint(int *pn)
{
  int b, c, sign;
  
  while (isspace(c = getch()))	/* skip white space */
    ;
  if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
    ungetch(c);  /* it is not a number */
    return 0;
  }
  sign = (c == '-') ? -1 : 1;
  if (c == '+' || c == '-') {
    b = c;
    if (!isdigit(c = getch())) {
      if (c != EOF)
	ungetch(c);
      ungetch(b);
      return '0';
    }
  }
  for (*pn = 0; isdigit(c); c = getch())
    *pn = 10 * *pn + (c - '0');
  *pn *= sign;
  if (c != EOF)
    ungetch(c);
  return c;
}

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
