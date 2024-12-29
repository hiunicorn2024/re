#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>		/* for  atof() */
#include <math.h>		/* for sin / exp / pow */

#define NUMBER  '0'		/* signal that a number was found */
int getop(char []);
void push(double);
double pop(void);
double top(void);
void clear();
int readline(int lim);

#define MAXOP   100		/* max size of operand or operator */
#define LINELIM 1000
int line_num;
char line[1000];

main()
{
  int type, linenum;
  double op2, op3, recp;
  char s[MAXOP];
  while (linenum = readline(LINELIM) > 0) {
    line_num = 0;
    while ((type = getop(s)) != EOF) {
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
      case 'R': 		/* print the most recently printed value */
	printf("\t%.8g\n", recp);
	break;
      case 'P': 		/* to print the top elements of the stack without popping */
	printf("\t%.8g\n", top());
	recp = top();
	break;
      case 'C': 		/* duplicate the top elements of the stack */
	op2 = pop();
	push(op2);
	push(op2);
	break;
      case 'S': 		/* to swap the top two elements */
	op2 = pop();
	op3 = pop();
	push(op2);
	push(op3);
	break;
      case '$': 		/* clear the stack */
	clear();
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
  }
  return 0;
}


#define MAXVAL  100		/* maximum depth of val stack */

int sp = 0;			/* next free stack position */
double val[MAXVAL];		/* value stack */

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
  extern char line[];
  extern line_num;
  int c, i;

  while ((c = line[line_num]) != '\0') {
    i = 0;
    if (c == ' ' || c == '\t')
      ++line_num;
    else if ((c == '-' || c == '+') && isdigit(line[line_num + 1]) || isdigit(c)) {
      if (c == '-') {
	s[i++] = c;
	++line_num;
      }
      while (isdigit(c = line[line_num])) {
	s[i++] = c;
	++line_num;
      }
      if ((c = line[line_num]) == '.') {
	s[i++] = c;
	++line_num;
      }
      while (isdigit(c = line[line_num])) {
	s[i++] = c;
	++line_num;
      }
      s[i] = '\0';
      return NUMBER;
    }
    else if (c == 's' && line[line_num + 1] == 'i' && line[line_num + 2] == 'n') {
      line_num += 3;
      return -2;
    }
    else if (c == 'e' && line[line_num + 1] == 'x' && line[line_num + 2] == 'p') {
      line_num += 3;
      return -3;
    }
    else if (c == 'p' && line[line_num + 1] == 'o' && line[line_num + 2] == 'w') {
      line_num += 3;
      return -4;
    }
    else {
      s[0] = c;
      s[1] = '\0';
      ++line_num;
      return c;
    }
  }
  return EOF;
}

int readline(int lim)
{
  extern char line[];
  int c, i = 0;

  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    line[i++] = c;
  if (c == '\n')
    line[i++] = c;
  line[i] = '\0';
  return i;
}
