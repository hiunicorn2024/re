#include <stdio.h>

#define STACKMAX 5000

main()
{
  int c, temp, i;
  int stack[STACKMAX], p;

  p = 0;
  c = EOF + 1;
  while (c != EOF && (c = getchar()) != EOF) {
    if (c == '\\')
      c = getchar();
    if (c == '\"') {
      stack[p] = '\"';
      ++p;
      while (c != EOF && (c = getchar()) != EOF && c != '\"')
	if (c == '\\')
	  c = getchar();
    } else if (c == '\'') {
      stack[p] = '\'';
      ++p;
      while (c != EOF && (c = getchar()) != EOF && c != '\'')
	if (c == '\\')
	  c = getchar();
    } else if (c == '(') {
      stack[p] = '(';
      ++p;
    } else if (c == '[') {
      stack[p] = '[';
      ++p;
    } else if (c == '{') {
      stack[p] = '{';
      ++p;
    } else if (c == ')') {
      if (stack[p - 1] == '(')
	--p;
      else
	printf("unmatched \')\'");
    } else if (c == ']') {
      if (stack[p - 1] == '[')
	--p;
      else
	printf("unmatched \']\'");
    } else if (c == '}') {
      if (stack[p - 1] == '{')
	--p;
      else
	printf("unmatched \'}\'");
    } else if (c == '/') {
      temp = c;
      if ((c = getchar()) == '*') {
	while ((c = getchar()) != '*' && c != EOF)
	  ;
	while (c != EOF && (c = getchar()) != '/')
	  while ((c = getchar()) != '*' && c != EOF)
	    ;
	if (c == EOF)
	  printf("uncompleted comment\n");
      }
    }
  }
  if (p != 0)
    printf("redundant '(', '[', or '{'\n");
}
