#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
struct tnode *addtree2(struct tnode *, char *, int);
void ftree(struct tnode *, struct tnode **);
void printtree(struct tnode *);

int main(void) {
  struct tnode *root;
  struct tnode *root2;
  char word[1000];
  root = root2 = NULL;
  int c;
  while ((c = getword(word, 1000)) != EOF)
    if (isalpha(word[0]))
      root = addtree(root, word);
  ftree(root, &root2);
  printtree(root2);
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	 "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  return 0;
}

struct tnode *addtree(struct tnode *p, char *w)
{
  int cond;
  if (p == NULL) {
    p = (struct tnode *)malloc(sizeof (struct tnode));
    if (p != NULL) {
      p->word = (char *)malloc(strlen(w) + 1);
      if (p->word != NULL)
	strcpy(p->word, w);
      p->count = 1;
      p->right = p->left = NULL;
    }
  }
  else if ((cond = strcmp(w, p->word)) == 0)
    ++p->count;
  else if (cond < 0)
    p->left = addtree(p->left, w);
  else
    p->right = addtree(p->right, w);
  return p;
}

struct tnode *addtree2(struct tnode *p, char *w, int count)
{
  if (p == NULL) {
    p = (struct tnode *)malloc(sizeof (struct tnode));
    if (p != NULL) {
      p->word = (char *)malloc(strlen(w) + 1);
      if (p->word != NULL)
	strcpy(p->word, w);
      p->count = count;
      p->right = p->left = NULL;
    }
  }
  else if (count <= p->count)
    p->right = addtree2(p->right, w, count);
  else
    p->left = addtree2(p->left, w, count);
  return p;
}

void ftree(struct tnode *p, struct tnode **p2) 
{
  if (p != NULL) {
    ftree(p->left, p2);
    *p2 = addtree2(*p2, p->word, p->count);
    ftree(p->right, p2);
  }
}

void printtree(struct tnode *p)
{
  static int bk = 0;
  if (p != NULL) {
    printtree(p->left);
    if (p->count != bk) {
      if (bk == 0)
	bk = p->count;
      printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	     "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      bk = p->count;
    }
    printf("%5d - %s\n", p->count, p->word);
    printtree(p->right);
  }
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
