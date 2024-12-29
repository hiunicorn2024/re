#include <stdio.h>
#include <ctype.h>

#define MAX 1000

rint readline(char *a, int lim);
int atoi_p(char s[]);
void itoa_p(char s[], int a);
void reverse_p(char s[]);
int strindex(char a[], char b[]);

main()
{
  char a[MAX];
  int c;
  if (c = readline(a, MAX))
    printf("*readline:\n%s%d\n", a, c);
  printf("*atoi:\n%d\n", atoi_p("250"));
  char s[MAX];
  itoa_p(s, 250);
  printf("*itoa:\n%s\n", s);
  char x[] = "abcdefg";
  char d[] = "def";
  printf("strindex:%d\n", strindex(x, d));
}

int readline(char *a, int lim) {
  char *m = a;
  int c;

  while (--lim && (c = getchar()) != EOF && c != '\n')
    *a++ = c;
  if (c == '\n')
    *a++ = c;
  *a = '\0';
  return (int)(a - m);
}

int atoi_p(char s[]) {
  int n = 0;
  int sign;
  sign = (*s == '-' ? -1 : 1);
  if (*s == '-' || *s == '+')
    ++s;
  while (isdigit(*s))
    n = n * 10 + (*s++) - '0';
  return n * sign;
}

void itoa_p(char s[], int a) {
  char *m = s;
  int sign = 0;
  if (a < 0) {
    a = -a;
    sign = 1;
  }
  do {
    *s++ = a % 10 + '0';
  } while (a /= 10);
  if (sign)
    *s++ = '-';
  *s = '\0';
  reverse_p(m);
}

void reverse_p(char s[]) {
  char temp, *i = s;
  while (*s++)
    ;
  s -= 2;
  while (s - i > 0) {
    temp = *s;
    *s-- = *i;
    *i++ = temp;
  }  
}

int strindex(char a[], char b[]) {
  char *c, *d, *e = a;
  while (*a) {
    for (c = b, d = a; *c && *d == *c; ++d, ++c)
      ;
    if (c > b && !*c)
      return a - e;
    ++a;
  }
}
