#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAXOP   100  /* max size of operand or operator */
#define NUMBER  '0'  /* signal that a number was found */

int getop(char []);
void push(double);
double pop(void);
void clear();
int sisdigit(char *);

/* reverse Polish calculator */
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
    case 'R': // print the most recently printed value
      printf("\t%.8g\n", recp);
      break;
    case 'P': // to print the top elements of the stack without popping
      op2 = pop();
      push(op2);
      printf("\t%.8g\n", op2);
      recp = op2;
      break;
    case 'C': // duplicate the top elements of the stack
      op2 = pop();
      push(op2);
      push(op2);
      break;
    case 'S': // to swap the top two elements
      op2 = pop();
      op3 = pop();
      push(op2);
      push(op3);
      break;
    case '$': // clear the stack
      clear();
      break;
    case EOF:
      return 0;
      break;
    case -2: // sin
      push(sin(pop()));
      break;
    case -3: // exp
      push(exp(pop()));
      break;
    case -4: // pow
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

/* push:  push f onto value stack */
void push(double f)
{
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

/* pop:  pop and return top value from stack */
double pop(void)
{
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

void clear() {
  sp = 0;
}

/* getop:  get next character or numeric operand */
int getop(char s[])
{
  int a;
 getop0:
  if ((a = scanf("%s", s)) != EOF) {
    if (strcmp(s, "sin") == 0 || strcmp(s, "SIN") == 0)
      return -2;
    else if (strcmp(s, "exp") == 0 || strcmp(s, "EXP") == 0)
      return -3;
    else if (strcmp(s, "pow") == 0 || strcmp(s, "POW") == 0)
      return -4;
    else if (sisdigit(s))
      return NUMBER;
    else
      return s[0];
  }
  else if (a == EOF)
    return EOF;
  else
    goto getop0;
}

int sisdigit(char *p) {
  if (*p == '+' || *p == '-')
    ++p;
  else if (!isdigit(*p))
    return 0;
  if (isdigit(*p)) {
    while (isdigit(*++p))
      ;
    if (!*p)
      return 1;
    else if (*p == '.') {
      if (isdigit(*++p)) {
	while (isdigit(*++p))
	  ;
	if (!*p)
	  return 1;
	else
	  return 0;
      }
      else
	return 0;
    }
    else
      return 0;
  }
  else
    return 0;
}
