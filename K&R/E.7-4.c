#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

int minscanf(char *format, ...);
int getch(void);
void ungetch(int);

int main(void)
{
  float a;
  char b[100];
  int c;
  printf("%d\n", minscanf("this is %f,%s %d", &a, b, &c));
  printf("\n%f\n%s\n%d\n", a, b, c);
  return 0;
} 

int minscanf(char *format, ...) {
  va_list ap;
  char *p, *sval;
  int *ival;
  float *fval;
  char *word[100];
  int i;
  int c;
  int num = 0;

  va_start(ap, format);
  for (p = format; *p; ++p) {
    if (isspace(*p)) {
      while (isspace(c = getch()))
	;
      ungetch(c);
    }
    else if (*p != '%') {
      i = 0;
      if ((c = getch()) != *p) {
	ungetch(c);
	return num;
      }
    }
    else {
      switch(*++p) {
      case 'd':
	ival = va_arg(ap, int *);
	{
	  int bk = 0;
	  int n = 0;
	  int sign;
	  sign = (c = getch()) == '-' ? -1 : 1;
	  if (c == '-' || c == '+') {
	    bk = c;
	    c = getch();
	  }
	  if (isdigit(c)) {
	    n = c - '0';
	    while (isdigit(c = getch()))
	      n = n * 10 + c - '0';
	    *ival = n * sign;
	    ++num;
	    ungetch(c);
	  }
	  else {
	    ungetch(c);
	    if (bk)
	      ungetch(bk);
	    return num;
	  }
	}
	break;
      case 'f':
	fval = va_arg(ap, float *);
	{
	  float n = 0;
	  int sign;
	  int pow = 1;
	  int bk = 0;
	  sign = (c = getch()) == '-' ? -1 : 1;
	  if (c == '-' || c == '+') {
	    bk = c;
	    c = getch();
	  }
	  if (isdigit(c)) {
	    n = c - '0';
	    while (isdigit(c = getch()))
	      n = n * 10 + c - '0';
	  }
	  else {
	    ungetch(c);
	    if (bk)
	      ungetch(bk);
	    return num;
	  }
	  if (c == '.') {
	    bk = c;
	    if (isdigit(c = getch())) {
	      n = n * 10 + c - '0';
	      pow *= 10;
	      while (isdigit(c = getch())) {
		n = n * 10 + c - '0';
		pow *= 10;
	      }
	      *fval = n * sign / pow;
	      ++num;
	      ungetch(c);
	    }
	    else {
	      ungetch(c);
	      ungetch(bk);
	      return num;
	    }
	  }
	  else {
	    *fval = n * sign / pow;
	    ++num;
	    ungetch(c);
	  }
	}
	break;
      case 's':
	sval = va_arg(ap, char *);
	while (isspace(c = getch()))
	  ;
	if (c != EOF) {
	  do {
	    *sval++ = c;
	  } while (!isspace(c = getch()) && c != EOF);
	  *sval = '\0';
	  ungetch(c);
	}
	else {
	  ungetch(c);
	  return num;
	}
	break;
      default:
	break;
      }
    }
  }
  va_end(ap);
  return num;
}

#define SIZE_buff 1000

int buff[SIZE_buff];
int buffp = 0;

int getch(void) {
  return buffp > 0 ? buff[--buffp] : getchar();
}

void ungetch(int m) {
  if (buffp >= SIZE_buff)
    printf("ERROR in ungetch: too many elements have been in stack\n");
  else
    buff[buffp++] = m;
}
