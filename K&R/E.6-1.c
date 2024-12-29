#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))

struct key {
  char *word;
  int count;
} keytab[] = {
  "auto", 0,
  "break", 0,
  "case", 0,
  "char", 0,
  "const", 0,
  "continue", 0,
  "default", 0,
  /* ... */
  "unsigned", 0,
  "void", 0,
  "volatile", 0,
  "while", 0
};

int getword(char *, int);
int binsearch(char *, struct key *, int);

/* count C keywords */
main()
{
  int n;
  char word[MAXWORD];

  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0]))
      if ((n = binsearch(word, keytab, NKEYS)) >= 0)
	keytab[n].count++;
  for (n = 0; n < NKEYS; n++)
    if (keytab[n].count > 0)
      printf("%4d %s\n",
	     keytab[n].count, keytab[n].word);
  return 0;
}

/* binsearch:  find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
  int cond;
  int low, high, mid;

  low = 0;
  high = n - 1;
  while (low <= high) {
    mid = (low+high) / 2;
    if ((cond = strcmp(word, tab[mid].word)) < 0)
      high = mid - 1;
    else if (cond > 0)
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim)
{
  int c, getch(void);
  void ungetch(int);
  char *w = word;

  while (isspace(c = getch()))
    ;
  if (c != EOF)
    *w++ = c;
  if (c == '\"') {
    while ((c = getch()) != '\"' && c != EOF)
      ;
    if (c == EOF)
      return EOF;
    else
      return '\"';
  }
  else if (c == '/') {
    if ((c = getch()) == '/') {
      while ((c = getch()) != '\n' && c != EOF)
	;
      if (c == EOF)
	return EOF;
      else
	return '/';
    }
    else if (c == '*') {
    m0:
      while ((c = getch()) != '*' && c != EOF)
	;
      if (c == EOF)
	return EOF;
      else {
	if ((c = getch()) == '/')
	  return '/';
	else
	  goto m0;
      }
    }
    else {
      ungetch(c);
      return '/';
    }
  }
  else if (c == '#') {
    while ((c = getch()) != '\n' && c != EOF)
      ;
    if (c == EOF)
      return EOF;
    else
      return '#';
  }
  if (!isalpha(c) && c != '_') {
    *w = '\0';
    return c;
  }    
  for ( ; --lim > 0; w++)
    if (!isalnum(*w = getch()) && *w != '_') {
      ungetch(*w);
      break;
    }
  *w = '\0';
  return word[0];
}

#define SIZE_buff 1000

int buff[SIZE_buff];
int buffp = 0;

int getch(void);
void ungetch(int);

int getch(void) {
  return buffp > 0 ? buff[--buffp] : getchar();
}

void ungetch(int m) {
  if (buffp >= SIZE_buff)
    printf("ERROR in ungetch: too many elements have been in stack\n");
  else
    buff[buffp++] = m;
}
