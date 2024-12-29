//COMMENT: The number of the input lines has limit.
#include <stdio.h>
#include <ctype.h>

#define MAX_line 500
#define MAX_alloc 40000
#define MAX_alloc_np 200

char alloc[MAX_alloc];
char *alloc_p = alloc;
char *alloc_np[MAX_alloc_np];
int alloc_np_p = 0;

int readlines(int lim);
int readline(char *a, int lim);

void printlines(int n);
int n_printline = 10;

main(int argc, char **argv)
{
  if (argc != 2) {
    printf("example: a.out -12\n");
    return;
  }
  if (*(*++argv) == '-') {
    int n = 0;
    while (isdigit(*++*argv))
      n = n * 10 + **argv - '0';
    n_printline = n;
  }

  if (readlines(MAX_alloc_np))
    printlines(n_printline);
}

int readlines(int lim) {
  int len;
  int i = 0;
  int n = 0;

  while ((len = readline(alloc_p, MAX_line)) > 0) {
    if (n + 1 <= lim) {
      *(alloc_np + i++) = alloc_p;
      alloc_p += len + 1;
      ++n;
    }
    else
      return alloc_np_p = -1;
  }
  return alloc_np_p = i;
}

void printlines(int n) {
  char **m;

  m = alloc_np + alloc_np_p - n < alloc_np ? alloc_np : alloc_np + alloc_np_p - n;
  while (m < (alloc_np + alloc_np_p))
    printf("%s", *m++);
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
