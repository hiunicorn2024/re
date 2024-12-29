#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>		/* for  atof() */
#include <math.h>		/* for sin / exp / pow */
#include <string.h>		/* for strlen() */

#define MAXOP   100		/* max size of operand or operator */
#define NUMBER  '0'		/* signal that a number was found */

int getop(char []);
void push(double);
double pop(void);
double top(void);
void clear();
void ungets(char s[]);		/* E.4-7 */
int getch_1c(void);		/* E.4-8 */
void ungetch_1c(int c);
int getop2(char s[]);		/* E.4-11 */
int getch(void);
void ungetch(int);

main()
{
  int type;
  double op2, op3, recp;
  char s[MAXOP];
  
  while (type = getop(s)) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      op2 = pop();
      push(pop() - op2);
      break;
    case '/':
      op2 = pop();
      if (op2 != 0.0)
	push(pop() / op2);
      else
	printf("error: zero divisor\n");
      break;
    case '\n':
      break;
    case '%':
      op2 = pop();
      push((int)pop() % (int)op2);
      break;
    case '=':
      printf("\t%.8g\n", recp = pop());
      break;
    case 'R': 			/* print the most recently printed value */
      printf("\t%.8g\n", recp);
      break;
    case 'P': 			/* to print the top elements of the stack without popping */
      printf("\t%.8g\n", top());
      recp = op2;
      break;
    case 'C': 			/* duplicate the top elements of the stack */
      op2 = pop();
      push(op2);
      push(op2);
      break;
    case 'S': 			/* to swap the top two elements */
      op2 = pop();
      op3 = pop();
      push(op2);
      push(op3);
      break;
    case '$': 			/* clear stack */
      clear();
      break;
    case -1:
      return 0;
      break;
    case -2: 			/* sin */
      push(sin(pop()));
      break;
    case -3: 			/* exp */
      push(exp(pop()));
      break;
    case -4: 			/* pow */
      op2 = pop();
      push(pow(pop(), op2));      
      break;
    default:
      printf("error: unknown command %s\n", s);
      break;
    }
  }
  return 0;
}

#define MAXVAL  100  /* maximum depth of val stack */

int sp = 0;          /* next free stack position */
double val[MAXVAL];  /* value stack */

void push(double f)
{
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

double top(void)
{
  if (sp > 0)
    return val[sp - 1];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

void clear() {
  sp = 0;
}

int getop(char s[])
{
  int i, c, b, a;
  
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  if (c == EOF)
    return -1;
  s[1] = '\0';
  if (!isdigit(c) && c != '.') {
    if (c == 's') {
      if ((b = getch()) == 'i') {
	if ((a = getch()) == 'n')
	  return -2;
	else {
	  ungetch(a);
	  ungetch(b);
	  return c;
	}
      }
      else {
	ungetch(b);
	return c;
      }
    }
    else if (c == 'e') {
      if ((b = getch()) == 'x') {
	if ((a = getch()) == 'p')
	  return -3;
	else {
	  ungetch(a);
	  ungetch(b);
	  return c;
	}
      }
      else {
	ungetch(b);
	return c;
      }
    }
    else if (c == 'p') {
      if ((b = getch()) == 'o') {
	if ((a = getch()) == 'w')
	  return -4;
	else {
	  ungetch(a);
	  ungetch(b);
	  return c;
	}
      }
      else {
	ungetch(b);
	return c;
      }
    }
    else {
      if (c != '-' && c != '+')
	return c;      /* not a number */
      else {
	if (isdigit(b = getch())) {
	  c = b;
	  s[i = 1] = b;
	label:
	  if (isdigit(c))    /* collect integer part */
	    while (isdigit(s[++i] = c = getch()))
	      ;
	  if (c == '.')      /* collect fraction part */
	    while (isdigit(s[++i] = c = getch()))
	      ;
	  s[i] = '\0';
	  ungetch(c);
	  return NUMBER;
	}
	else
	  return c;
      }
    }
  }
  i = 0;
  goto label;
}

#define BUFSIZE 100

int buf[BUFSIZE];		/* buffer for ungetch */
int bufp = 0;			/* next free position in buf */

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

void ungets(char s[]) {
  int i = strlen(s);

  if (i > 0)
    do {
      ungetch(s[(i--) - 1]);
    } while (i >= 1);
}

int m;

int getch_1c(void)
{
  int c;

  if (m != EOF) {
    c = m;
    m = EOF;
    return c;
  }
  else
    return getchar();
}

void ungetch_1c(int c)
{
  if (m != EOF)
    printf("ungetch: too many characters\n");
  else
    m = c;
}

int getop2(char s[])
{
  int i;
  int c;
  static int m = '0';

  if (m == '0')  
    while ((s[0] = c = getchar()) == ' ' || c == '\t')
      ;
  else {
    if (m == ' ' || m == '\t')
      while ((s[0] = c = getchar()) == ' ' || c == '\t')
	;
    else
      s[0] = c = m;
    m = '0';
  }
  s[1] = '\0';
  if (!isdigit(c) && c != '.')
    return c;      /* not a number */
  i = 0;
  if (isdigit(c))    /* collect integer part */
    while (isdigit(s[++i] = c = getchar()))
      ;
  if (c == '.')      /* collect fraction part */
    while (isdigit(s[++i] = c = getchar()))
      ;
  s[i] = '\0';
  m = c;
  return NUMBER;
}
