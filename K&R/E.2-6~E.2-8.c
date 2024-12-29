#include <stdio.h>

#define MAXNUM 1000

unsigned setbits(unsigned x, int p, int n, unsigned y);
unsigned invert(unsigned x, int p, int n);
unsigned rightrot(unsigned x, int n);
int readline(char s[], int lim);
unsigned stoi_u(char s[]);
void print_itob_u(unsigned x);
void reverse(char a[]);

main()
{
  char a[MAXNUM];
  unsigned x, y;
  int p, n;

  printf("E.2-6 setbits(x, p, n, y):\n~~~~~\n");
  printf("x:\n");
  readline(a, MAXNUM);
  x = stoi_u(a);
  print_itob_u(x);
  printf("p:\n");
  readline(a, MAXNUM);
  p = stoi_u(a);
  printf("n:\n");
  readline(a, MAXNUM);
  n = stoi_u(a);
  printf("y:\n");
  readline(a, MAXNUM);
  y = stoi_u(a);
  print_itob_u(y);
  printf("~~~~~\nresult:\n");
  print_itob_u(setbits(x, p, n, y));
  
  printf("\n\nE.2-7 invert(x, p, n):\n~~~~~\n");
  printf("x:\n");
  readline(a, MAXNUM);
  x = stoi_u(a);
  print_itob_u(x);
  printf("p:\n");
  readline(a, MAXNUM);
  p = stoi_u(a);
  printf("n:\n");
  readline(a, MAXNUM);
  n = stoi_u(a);
  printf("~~~~~\nresult:\n");
  print_itob_u(invert(x, p, n));

  printf("\n\nE.2-8 rightrot(x, n):\n~~~~~\n");
  printf("x:\n");
  readline(a, MAXNUM);
  x = stoi_u(a);
  print_itob_u(x);
  printf("n:\n");
  readline(a, MAXNUM);
  n = stoi_u(a);
  printf("~~~~~\nresult:\n");
  print_itob_u(rightrot(x, n));
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
  return (x & ((~0 << (p + 1)) | ~(~0 << (p + 1 - n))) | ((y & ~(~0 << n)) << (p + 1 - n)));
}

unsigned invert(unsigned x, int p, int n)
{
  return x ^ (~(~0 << n) << p);
}

unsigned rightrot(unsigned x, int n)
{
  return ((~(~0 << n) & x) << (32 - n)) | (x >> n);
}

int readline(char s[], int lim)
{
  int c, i = 0, j = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    if (j < lim - 1)
      s[i++] = c;
    ++j;
  }
  if (j < lim - 1 && c == '\n')
    s[i++] = '\n';
  s[i] = '\0';
  return j;
}

unsigned stoi_u(char s[]) {
  unsigned x = 0;
  int i = 0;

  while (s[i] != '\0') {
    if (s[i] <= '9' && s[i] >= '0') {
      x = x * 10 + s[i] - '0';
    }
    ++i;
  }
  return x;
}

void print_itob_u(unsigned x) {
  char s[MAXNUM];
  unsigned m;

  m = x;
  int i = 0;
  while (m != 0) {
    s[i] =  m % 2 + '0';
    m = m / 2;
    ++i;
  }
  s[i] = '\0';
  reverse(s);
  printf("%s\n", s);
}

void reverse(char a[]) {
  int i = 0, n = 0, m, x;

  while (a[i++] != '\0')
    ++n;
  m = n / 2;
  for (i = 0; i < m; ++i) {
    x = a[i];
    a[i] = a[n - 1 - i];
    a[n - 1 - i] = x;
  }
}
