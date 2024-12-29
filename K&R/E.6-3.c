#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 1000

struct tnode {     /* the tree node: */
  char *word;           /* points to the text */
  struct subtnode *linum;
  struct tnode *left;   /* left child */
  struct tnode *right;  /* right child */
};

struct subtnode {
  int linum;
  struct subtnode *left;
  struct subtnode *right;
};

int linum = 1;
struct tnode *addtree(struct tnode *, char *);
struct subtnode *addsubtree(struct subtnode *, int);
void treeprint(struct tnode *);
void subtreeprint(struct subtnode *);
int getword(char *, int);

/* word frequency count */
int main(void)
{
  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  while (getword(word, MAXWORD) != EOF)
    if (isalpha(word[0]))
      root = addtree(root, word);
  treeprint(root);
  return 0;
}

struct tnode *talloc(void);
struct subtnode *talloc2(void);
char *strdup_0(char *);

struct tnode *addtree(struct tnode *p, char *w)
{
  int cond;
  if (p == NULL) {
    p = talloc();
    p->word = strdup_0(w);
    p->linum = addsubtree(p->linum, linum);
    p->left = p->right = NULL;
  } 
  else if ((cond = strcmp(w, p->word)) == 0)
    p->linum = addsubtree(p->linum, linum);
  else if (cond < 0)
    p->left = addtree(p->left, w);
  else
    p->right = addtree(p->right, w);
  return p;
}

struct subtnode *addsubtree(struct subtnode *p, int l)
{
  if (p == NULL) {
    p = talloc2();
    p->linum = l;
    p->left = p->right = NULL;
  }
  else if (l == p->linum)
    ;
  else if (l < p->linum)
    p->left = addsubtree(p->left, l);
  else
    p->right = addsubtree(p->right, l);
  return p;
}

void subtreeprint(struct subtnode *p)
{
  if (p != NULL) {
    subtreeprint(p->left);
    printf("\t%d\n", p->linum);
    subtreeprint(p->right);
  }
}

void treeprint(struct tnode *p)
{
  if (p != NULL) {
    treeprint(p->left);
    printf("%s\n", p->word);
    subtreeprint(p->linum);
    treeprint(p->right);
  }
}

struct tnode *talloc(void)
{
  return (struct tnode *)malloc(sizeof (struct tnode));
}

struct subtnode *talloc2(void)
{
  return (struct subtnode *)malloc(sizeof (struct subtnode));
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

  while ((c = getch()) == ' ' || c == '\t')
    ;
  if (c != EOF)
    *w++ = c;
  if (c == '\n')
    ++linum;
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
  if (strcmp(word, "and") == 0 || strcmp(word, "the") == 0||
      strcmp(word, "or") == 0 || strcmp(word, "however") == 0)
    word[0] = '\0';
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
