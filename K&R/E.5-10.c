#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Stacks of string and double */
#define MAX_stack_p 1000
#define MAX_alloc 10000

/* expression */
char alloc0[MAX_alloc] = "\0";
char *pn_alloc0[MAX_stack_p];
char *p_alloc0 = alloc0;

/* number */
double alloc1[MAX_stack_p];
double *p_alloc1 = alloc1;
double pop_num(char *);
void push_num(double, char *, int);

int judge_str(char *, int *);
char *connection(char *, char *, char, int *);

int main(int argc, char *argv[])
{
  char buff0[MAX_alloc];
  char buff1[MAX_alloc];
  double buff_double_0;

  if (argc < 2) {
    printf("expr 2 3 4 + *\n");
    return -1;
  }
  while (--argc) {
    int n = 0, len = 0;
    char *c, *a;
    double m, op2;
    c = *(++argv);
    switch(judge_str(c, &len)) {
    case 0: // number
      push_num(atof(c), c, len);
      break;
    case '*':
      m = pop_num(buff1) * pop_num(buff0);
      a = connection(buff0, buff1, '*', &len);
      push_num(m, a, len);
      break;
    case '/':
      op2 = pop_num(buff1); 
      m = pop_num(buff0) / op2;
      a = connection(buff0, buff1, '/', &len);
      push_num(m, a, len);
      break;
    case '+':
      m = pop_num(buff1) + pop_num(buff0);
      a = connection(buff0, buff1, '+', &len);
      push_num(m, a, len);
      break;
    case '-':
      op2 = pop_num(buff1); 
      m = pop_num(buff0) - op2;
      a = connection(buff0, buff1, '-', &len);
      push_num(m, a, len);
      break;
    default:
      printf("ERROR INPUT\n");
      return -1;
      break;
    }
    ++c;
  }
  buff_double_0 = pop_num(buff0);
  printf("left:%s\n", alloc0);
  printf("used:%s = %f\n", buff0, buff_double_0);
  return 0;
}

int judge_str(char *s, int *len) {
  int m = 0;
  char *u = s;
  if ((*s == '+' || *s == '-') && isdigit(*(s + 1)) || isdigit(*s)) {
    ++m;
    ++s;
    while(isdigit(*s)) {
      ++s;
      ++m;
    }
    if (*s == '\0') {
      *len = m;
      return 0;
    }
    if (*s != '.')
      return -1;
    ++s;
    ++m;
    while(isdigit(*s)) {
      ++s;
      ++m;
    }
    if (*s != '\0') {
      return -1;
    }
    *len = m;
    return 0;
  }
  else if (*(u + 1) == '\0')
    return *u;
  else
    return -1;
}

double pop_num(char *s) {
  if (p_alloc1 == alloc1) {
    printf("ERROR: The stack is empty.\n");
    return 0;
  }
  strcpy(s, p_alloc0 = *(pn_alloc0 + ((p_alloc1--) - alloc1 - 1)));
  *p_alloc0 = '\0';
  return *p_alloc1;
}

void push_num(double a, char *s, int len) {
  if (p_alloc0 - alloc0 + len > MAX_alloc || p_alloc1 - alloc1 >= MAX_stack_p) {
    printf("ERROR: The stack has been full.\n");
    return;
  }
  *p_alloc1 = a;
  *(pn_alloc0 + ((p_alloc1++) - alloc1)) = p_alloc0;
  strcat(alloc0, s);
  p_alloc0 += len;
}

char *connection(char *a, char *b, char m, int *len) {
  char *x = a;
  char *y;

  while (*a++)
    ;
  y = a;
  while (a > x)
    *a-- = *(a - 1);
  *x = '(';
  *y++ = ' ';
  *y++ = m;
  *y++ = ' ';
  while (*b)
    *y++ = *b++;
  *y++ = ')';
  *y = '\0';
  *len = y - x;
  return x;
}
