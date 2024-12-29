#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 1000

struct tnode {     /* the tree node: */
  char *word;           /* points to the text */
  struct tnode *subtree;
  struct tnode *left;   /* left child */
  struct tnode *right;  /* right child */
};

struct tnode *addtree(struct tnode *, char *, int state);
void treeprint(struct tnode *);
void subtreeprint(struct tnode *);
int getword(char *, int);

/* word frequency count */
main(int argc, char **argv)
{
  int state = 6;
  if (argc > 2)
    printf("the program need only 0 or 1 unsigned integer argument (>=1).");
  if (argc == 2 && isdigit(**++argv)) {
    int i = 0;
    while (isdigit(**argv)) {
      i = i * 10 + **argv - '0';
      ++*argv;
    }
    if (**argv || i == 0)
      printf("the program need only 0 or 1 digit argument.");
    else
      state = i;
  }
  
  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0]))
      root = addtree(root, word, state);
  subtreeprint(root);
  return 0;
}

struct tnode *talloc(void);
char *strdup_0(char *);

struct tnode *addtree(struct tnode *p, char *w, int state)
{
  int cond;

  if (p == NULL) {
    p = talloc();
    p->word = strdup_0(w);
    p->subtree = p->left = p->right = NULL;
    return p;
  } 

  if (state == 0)
    cond = strcmp(w, p->word);
  else
    cond = strncmp(w, p->word, state);

  if (cond == 0) {
    if (state > 0)
      p->subtree = addtree(p->subtree, w, 0);
  }
  else if (cond < 0)
    p->left = addtree(p->left, w, state);
  else
    p->right = addtree(p->right, w, state);
  return p;
}

void subtreeprint(struct tnode *p)
{
  if (p != NULL) {
    subtreeprint(p->left);
    if (p->subtree != NULL) {
      printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	     "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      printf("%s\n", p->word);
      treeprint(p->subtree);
    }
    subtreeprint(p->right);
  }
}

void treeprint(struct tnode *p)
{
  if (p != NULL) {
    treeprint(p->left);
    printf("%s\n", p->word);
    treeprint(p->right);
  }
}

/* talloc:  make a tnode */
struct tnode *talloc(void)
{
  return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup_0(char *s)   /* make a duplicate of s */
{
  char *p;

  p = (char *) malloc(strlen(s)+1); /* +1 for '\0' */
  if (p != NULL)
    strcpy(p, s);
  return p;
}

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
