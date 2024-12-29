#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 500
#define MAXLINES 5000     /* max #lines to be sorted */
#define ALLOCSIZE 10000 /* size of available space */

int isletter(char);
static char allocbuf[ALLOCSIZE]; /* storage for alloc */
static char *allocp = allocbuf;  /* next free position */
char *lineptr[MAXLINES];  /* pointers to text lines */
int readline(char *, int);
char *alloc(int);
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines, int r_state);
void qsort_p(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);
int strcmp1(char *, char *);
int strcmp0(char *, char *);
int d_state = 0;  /* makes comparisons only on letters, numbers and blanks */

/* sort input lines */
main(int argc, char *argv[])
{
  int nlines;        /* number of input lines read */
  int numeric = 0;   /* 1 if numeric sort */
  int r_state = 0;
  int f_state = 0;  /* fold upper and lower case together, so that case
		    ** distinctions are not made during sorting; for example,
		    ** a and A compare equal.
		    */
  int a = 0, b = 0;
  if (argc > 1) {
    ++argv;
    while (--argc) {
      if (**argv == '-') {
	++*argv;
	while (**argv) {
	  if (**argv == 'r')
	    r_state = 1;
	  else if (**argv == 'n')
	    numeric = 1;
	  else if (**argv == 'f')
	    f_state = 1;
	  else if (**argv == 'd')
	    d_state = 1;
	  else
	    ;
	  ++(*argv);
	}
      } 
      else if (**argv == ']') {
	++*argv;
	a = 0;
	while (isdigit(**argv)) {
	  a = a * 10 + **argv - '0';
	  ++*argv;
	}
	if (**argv)
	  a = 0;
      } 
      else if (**argv == '[') {
	++*argv;
	b = 0;
	while (isdigit(**argv)) {
	  b = b * 10 + **argv - '0';
	  ++*argv;
	}
	if (**argv)
	  b = 0;
      }
      ++argv;
    }
  }
  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    int left = 1;
    int right = nlines;
    if (a > 0 && b > 0 && a < b && b <= nlines) {
      left = a;
      right = b;
    }
    if (f_state == 1)
      qsort_p((void**) lineptr, left - 1, right -1,
	      (numeric ? (int (*)(void*,void*))numcmp : (int (*)(void*,void*))strcmp1));
    else
      qsort_p((void**) lineptr, left - 1, right - 1,
	      (numeric ? (int (*)(void*,void*))numcmp : (int (*)(void*,void*))strcmp0));
    writelines(lineptr, nlines, r_state);
    return 0;
  } 
  else {
    printf("input too big to sort\n");
    return 1;
  }
}

/* qsort:  sort v[left]...v[right] into increasing order */
void qsort_p(void *v[], int left, int right, int (*comp)(void *, void *))
{
  int i, last;
  
  void swap(void *v[], int, int);
  
  if (left >= right)    /* do  nothing if array contains */
    return;           /* fewer than two elements */
  swap(v, left, (left + right)/2);
  last = left;
  for (i = left+1; i <= right;  i++)
    if ((*comp)(v[i], v[left]) < 0)
      swap(v, ++last, i);
  swap(v, left, last);
  qsort_p(v, left, last-1, comp);
  qsort_p(v, last+1, right, comp);
}

#include <stdlib.h>

/* numcmp:  compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
  double v1, v2;
  
  v1 = atof(s1);
  v2 = atof(s2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return 0;
}

int strcmp0(char *a, char *b) {
  if (d_state) {
    while (((*a == *b) || !(isdigit(*a) || *a == ' ' || *a == '\t' || isletter(*a)) || !(isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b))) && *a && *b) {
      if ((isdigit(*a) || *a == ' ' || *a == '\t' || isletter(*a)) & (isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++a, ++b;
      else if ((isdigit(*a) || *a == ' ' || *a == '\t' || isletter(*a)) && !(isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++b;
      else if (!(isdigit(*a) || *a == ' ' || *a == '\t' || isletter(*a)) && (isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++a;
      else
	++a, ++b;
  }
    if (!*b && !*a)
      return 0;
    else
      return *a - *b;
  }
  else {
    while (*a == *b && *a && *b)
      ++a, ++b;
    if (!*b && !*a)
      return 0;
    else
      return *a - *b;
  }
}

int strcmp1(char *a, char *b) {
  if (d_state) {
    while (((*a & 0xDF) == (*b & 0xDF)
	    || !(isdigit(*a) || *a == ' '|| *a == '\t' || isletter(*a))
	    || !(isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b))) && *a && *b) {
      if ((isdigit(*a) || *a == ' ' || *a == '\t'
	   || isletter(*a)) && (isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++a, ++b;
      else if ((isdigit(*a) || *a == ' ' || *a == '\t'
		|| isletter(*a)) && !(isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++b;
      else if (!(isdigit(*a) || *a == ' ' || *a == '\t' || isletter(*a))
	       && (isdigit(*b) || *b == ' ' || *b == '\t' || isletter(*b)))
	++a;
      else
	++a, ++b;
    }
    if (!*b && !*a)
      return 0;
    else
      return (*a & 0xDF) - (*b & 0xDF);
  }
  else {
    while ((*a & 0xDF) == (*b & 0xDF) && *a && *b)
      ++a, ++b;
    if (!*b && !*a)
      return 0;
    else
      return (*a & 0xDF) - (*b & 0xDF);
  }
}

void swap(void *v[],  int i, int j)
{
  void *temp;
  
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[MAXLEN];
  
  nlines = 0;
  while ((len = readline(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (p = alloc(len)) == NULL)
      return -1;
    else {
      line[len-1] = '\0';  /* delete newline */
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
  return nlines;
}

void writelines(char *lineptr[], int nlines, int r_state)
{
  int i;

  if (r_state) {
    for (i = nlines - 1; i >= 0; --i)
      printf("%s\n", lineptr[i]);
  }
  else {
    for (i = 0; i < nlines; i++)
      printf("%s\n", lineptr[i]);
  }
}

int readline(char *line, int max) {
  int c;
  char *s = line;

  while (--max && (c = getchar()) != EOF && c != '\n')
    *line++ = c;
  if (c == '\n')
    *line++ = c;
  *line = '\0';
  return line - s;
}

char *alloc(int n)    /* return pointer to n characters */
{
  if (allocbuf + ALLOCSIZE - allocp >= n) {  /* it fits */
    allocp += n;
    return allocp - n; /* old p */
  } else      /* not enough room */
    return 0;
}

int isletter(char m)
{
  return (m <= 0x5A && m >= 0x41 || m <= 0x7A && m >= 0x61) ? 1 : 0;
}
