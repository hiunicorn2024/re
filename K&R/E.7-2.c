#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum {HEXADECIMAL, OCTAL};

int linelen = 80;
int tablen = 8;
#define TABLEN(j) (linelen - (j) % linelen <= tablen ?  \
		   tablen - 2 - (j) % tablen :		\
		   tablen - 1 - (j) % tablen) + 1

int linelim = 10;

int main(int argc, char **argv)
{
  int state = 0;
  while (--argc) {
    if (**++argv == '-') {
      if (strcmp(++*argv, "o") == 0 || strcmp(++*argv, "O") == 0)
	state = OCTAL;
    }
  }

  char format[20];
  strcpy(format, state == OCTAL ? "\'\\%3.3o\'" : "\'\\X%2.2X\'");
  int c;
  char a[10000];
  int i = 0;

  while ((c = getchar()) != EOF)
    a[i++] = c;
  a[i] = '\0';
  i = 0;  

  int row = 0;
  char *bk = &a[i];

  while ((c = a[i]) != '\0') {
    if (row == 0)
      bk = &a[i];
    if (isalnum(c) || c == '~' || c == '`' || c =='!' || c == '@' || c =='#' ||
	c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' ||
	c == ')' || c == '-' || c == '+' || c == '[' || c == ']' || c == '{' ||
	c == '}' || c == '|' || c == '\\' || c == ':' || c == ';' || c == '\'' ||
	c == '\"' || c == '<' || c == ',' || c == '.' || c == '>' || c == '?' ||
	c == '/' || c == '\t' || c == ' ' || c == '\n') {
      if (row >= linelim) {
	int i_bk = i;
	int i_l;
	if (a[--i] == ' ' || a[i] == '\t') {
	  while (&a[--i] >= bk && (a[i] == ' ' || a[i] == '\t'))
	    ;
	  if (&a[i] < bk) {
	    i = i_bk;	    
	    row = 0;
	  }
	  else {
	    i_l = i;
	    printf("%.*s\n", (int)(&a[i_l] - bk) + 1, bk);
	    row = 0;
	  }
	  i = i_bk;
	  while (a[++i] != '\0' && (a[i] == ' ' || a[i] == '\t'))
	    ;
	  if (a[i] == '\0')
	    break;
	  else
	    continue;
	}
	else if (!isspace(a[i + 1])) {
	  while (&a[i] >= bk && (a[i] != ' ' && a[i] != '\t'))
	    --i;
	  if (&a[i] < bk) {
	    printf("%.*s-\n", linelim - 1, bk);
	    i = i_bk - 1;
	  }
	  else {
	    i_bk = i;
	    while (a[--i] == ' ' && a[i] == '\t')
	      ;
	    printf("%.*s\n", (int)(&a[i] - bk) + 1, bk);
	    i = i_bk + 1;
	  }
	  row = 0;
	  continue;
	}
	else {
	  printf("%.*s\n", linelim, bk);
	  while (a[++i] == ' ' || a[i] == '\t')
	    ;
	  if (a[i] == '\n')
	    ++i;
	  row = 0;
	  continue;
	}
      }

      if (c == '\t')
	row += TABLEN(row);
      else if (c == '\n') {
	printf("%.*s", (int)(&a[i] - bk) + 1, bk);
	row = 0;
      }
      else
	++row;
      ++i;
    }
    else {
      row = 0;
      putchar('\n');
      printf("* non-graphic character:");
      printf(format, c);
      putchar('\n');
      ++i;
    }
  }
  if (row == 0)
    bk = &a[i];
  if (bk < &a[i])
    printf("%s", bk);
  return 0;
}
