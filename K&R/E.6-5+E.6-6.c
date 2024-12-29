#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nlist {       
  struct nlist *next;   
  char *name;           
  char *defn;           
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE];  

struct nlist *install(char *name, char *defn);
unsigned hash(char *s);
struct nlist *lookup(char *);
char *strdup_0(char *);
int getword(char *word, int lim);
void defprint(struct nlist **tab, int lim);
int undef(struct nlist **tab, int lim, char *word);
void macro(char *file, struct nlist **tab, int lim);
char *tabwordcmp(struct nlist **tab, int lim, char *w);

char filebuff[20000];
int filebuff_i = 0;

int main(void)
{
  char word[200];
  char word2[200];
  int c;
  int i;
  while ((c = getword(word, 200)) != EOF) {
    if (c == '#' && isalpha(word[0])) {
      for (i = 0; word[i] != ' '; ++i)
	;
      word[i] = '\0';
      strcpy(word2, word + i + 1);
      install(word, word2);
    }
  }
  char *a;
  if ((a = tabwordcmp(hashtab, HASHSIZE, "HASHSIZE")) != NULL)
    printf("%s\n", a);
  //  defprint(hashtab, HASHSIZE);
  macro(filebuff, hashtab, HASHSIZE);
  printf("%s", filebuff);
  return 0;
}

struct nlist *install(char *name, char *defn)
{
  struct nlist *np;
  unsigned hashval;

  if ((np = lookup(name)) == NULL) { 
    np = (struct nlist *) malloc(sizeof(*np));
    if (np == NULL || (np->name = strdup_0(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;
  }
  else       
    free((void *) np->defn);   
  if ((np->defn = strdup(defn)) == NULL)
    return NULL;
  return np;
}

unsigned hash(char *s)
{
  unsigned hashval;

  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31 * hashval;
  return hashval % HASHSIZE;
}

char *strdup_0(char *s)
{
  char *p;

  p = (char *) malloc(strlen(s)+1);
  if (p != NULL)
    strcpy(p, s);
  return p;
}

struct nlist *lookup(char *s)
{
  struct nlist *np;

  for (np = hashtab[hash(s)]; np != NULL; np = np->next)
    if (strcmp(s, np->name) == 0)
      return np;
  return NULL;           
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
    if ((c = getch()) == '#') {
    w0:
      while ((c = getch()) != '\n' && c != EOF)
	;
      if (c == EOF)
	return EOF;
      else {
	word[0] = '\0';
	return '#';
      }
    }
    else if (c == 'd' && (c = getch()) == 'e' && (c = getch()) == 'f' &&
	     (c = getch()) == 'i' && (c = getch()) == 'n' && (c = getch()) == 'e') {
      if (isalpha(getword(word, lim))) {
	char *w1;
	w1 = (char *)malloc(lim * sizeof ('\0'));
	char *w2;
	w2 = (char *)malloc(lim * sizeof ('\0'));
	strcpy(w1, word);
	
	int i = 0;
	while ((c = getch()) == ' ' || c == '\t')
	  ;
	word[i++] = c;
	if (c != '\n') {
	  while (--lim) {
	    word[i] = getch();
	    if (isspace(word[i])) {
	      ungetch(word[i]);
	      if (word[i] != '\n') {
		word[0] = '\0';
		return '#';
	      }
	      word[i] = '\0';
	      break;
	    }
	    ++i;
	  }
	}
	else {
	  ungetch(c);
	  goto w0;
	}
	strcpy(w2, word);
	strcpy(word, w1);
	strcat(word, " ");
	strcat(word, w2);
	free((void *) w1);
	free((void *) w2);
	return '#';
      }
      else
	goto w0;
    }
    else {
      ungetch(c);
      goto w0;
    }
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

void defprint(struct nlist **tab, int lim) {
  int i;
  static struct nlist *k;
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	 "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  for (i = 0; i < lim; ++i) {
    if (*(tab + i) != NULL) {
      printf("%s\nDEF:\t%s\n", (**(tab + i)).name, (**(tab + i)).defn);
      k = *(tab + i);
      while (k->next != NULL) {
	k = k->next;
	printf("%s\nDEF:\t%s\n", k->name, k->defn);
      }
    }
  }
}

int undef(struct nlist **tab, int lim, char *word) {
  int i;
  static struct nlist *k;
  for (i = 0; i < lim; ++i) {
    if (*(tab + i) != NULL) {
      if (strcmp((**(tab + i)).name, word) == 0) {
	*(tab + i) = (**(tab + i)).next;
	return 1;
      }
      k = *(tab + i);
      while (k->next != NULL) {
	if (strcmp(k->next->name, word) == 0) {
	  k->next = k->next->next;
	  return 1;
	}
	k = k->next;
      }
    }
  }
  return 0;
}

void macro(char *file, struct nlist **tab, int lim) {
  char *filebk = file;
  char *bk = (char *)malloc(strlen(file) * 2);
  int bk_i = 0;
  char word[200];
  int word_i = 0;
  int incmt1 = 0;
  int incmt2 = 0;
  int inqut = 0;
  while (*file != '\0') {
    if (*file == '#' || (*file == '/' && *(file + 1) == '/')) {
      if (!incmt1 && !incmt2 && !inqut)
	incmt1 = 1;
      if (*file == '#')
	bk[bk_i++] = *file++;
      else {
	bk[bk_i++] = *file++;
	bk[bk_i++] = *file++;
      }
    }
    else if (*file == '/' && *(file + 1) == '*') {
      if (!incmt2 && !incmt1 && !inqut)
	incmt2 = 1;
      bk[bk_i++] = *file++;
      bk[bk_i++] = *file++;
    }
    else if (*file == '*' && *(file + 1) == '/') {
      if (incmt2)
	incmt2 = 0;
      bk[bk_i++] = *file++;
      bk[bk_i++] = *file++;
    }
    else if (*file == '\n') {
      if (incmt1)
	incmt1 = 0;
      bk[bk_i++] = *file++;
    }
    else if (*file == '\"') {
      if (inqut)
	inqut = 0;
      else if (!incmt1 && !incmt2)
	inqut = 1;
      bk[bk_i++] = *file++;
    }
    else if (!incmt1 && !incmt2 && !inqut && (isalpha(*file) || *file == '_')) {
      //char *tabwordcmp(struct nlist **tab, int lim, char *w);
      int bk_i_bk = bk_i;
      word[word_i++] =bk[bk_i++] = *file++;
      while (isalpha(*file) || *file == '_') {
	word[word_i++] =bk[bk_i++] = *file++;
      }
      word[word_i] = '\0';
      word_i = 0;
      char *m;
      if ((m = tabwordcmp(hashtab, HASHSIZE, word)) != NULL) {
	bk_i = bk_i_bk;
	bk[bk_i] = '\0';
	strcat(bk, m);
	while (bk[++bk_i] != '\0')
	  ;
      }
    }
    else
      bk[bk_i++] = *file++;
  }
  bk[bk_i] = '\0';
  strcpy(filebk, bk);
}

/*
struct nlist {       
  struct nlist *next;   
  char *name;           
  char *defn;           
};
*/

char *tabwordcmp(struct nlist **tab, int lim, char *w) {
  while (--lim >= 0) {
    if (tab[lim] != NULL) {
      if (strcmp(w, tab[lim]->name) == 0) {
	return tab[lim]->defn;
      }
      else {
	struct nlist *bk = tab[lim];
	while (bk->next != NULL) {
	  if (strcmp(w, bk->next->name) == 0) {
	    return bk->next->defn;
	  }
	  bk = bk->next;
	}
      }
    }
  }
  return NULL;
}

#define SIZE_buff 1000

int buff[SIZE_buff];
int buffp = 0;

int getch(void);
void ungetch(int);

int getch(void) {
  int c;
  c = buffp > 0 ? buff[--buffp] : getchar();
  filebuff[filebuff_i++] = c;
  if (c == EOF)
    filebuff[--filebuff_i] = '\0';
  return c;
}

void ungetch(int m) {
  if (buffp >= SIZE_buff)
    printf("ERROR in ungetch: too many elements have been in stack\n");
  else {
    --filebuff_i;
    buff[buffp++] = m;
  }
}
