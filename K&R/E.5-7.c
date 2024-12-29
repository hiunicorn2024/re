#include <stdio.h>
#include <string.h>

#define MAXLINES 5000     /* max #lines to be sorted */

char *lineptr[MAXLINES];  /* pointers to text lines */

int readlines(char *lineptr[], int maxlines, char alloc[]);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

#define MAXALLOC 10000
/* sort input lines */
main()
{
  int nlines;     /* number of input lines read */
  char alloc[MAXALLOC];
  
  if ((nlines = readlines(lineptr, MAXLINES, alloc)) >= 0) {
    qsort(lineptr, 0, nlines-1);
    writelines(lineptr, nlines);
    return 0;
  } 
  else {
    printf("error: input too big to sort\n");
    return 1;
  }
}

#define MAXLEN 1000  /* max length of any input line */
int readline(char *s, int lim);

/* readlines:  read input lines */
int readlines(char *lineptr[], int maxlines, char alloc[])
{
  int len, nlines, m = 0;
  char *p, line[MAXLEN];
  
  nlines = 0;
  while ((len = readline(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (p = alloc + m) >= (alloc + MAXALLOC))
      return -1;
    else {
      m += len;
      line[len-1] = '\0';  /* delete newline */
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
  return nlines;
}

void writelines(char *lineptr[], int nlines)
{
  while (nlines--)
    printf("%s\n", *lineptr++);
}

void swap(char *v[], int i, int j);

void qsort(char *v[], int left, int right)
{
  int i, last;
  
  if (left >= right)  /* do nothing if array contains */
    return;         /* fewer than two elements */
  swap(v, left, (left + right)/2);
  last = left;
  for (i = left+1; i <= right; i++)
    if (strcmp(v[i], v[left]) < 0)
      swap(v, ++last, i);
  swap(v, left, last);
  qsort(v, left, last-1);
  qsort(v, last+1, right);
}

/* swap:  interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
  char *temp;
  
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

int readline(char *s, int lim) {
  int c;
  char *m = s;

  while (--lim && (c = getchar()) != EOF && c != '\n')
    *s++ = c;
  if (c == '\n')
    *s++ = c;
  *s = '\0';
  return s - m;
}
