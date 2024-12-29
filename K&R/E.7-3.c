#include <stdarg.h>
#include <stdio.h>

void minprintf(char *fmt, ...);

int main(void)
{
  minprintf("%-15f\n", 100.0);
}

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
  va_list ap; /* points to each unnamed arg in turn */
  char *p, *sval;
  int ival;
  double dval;
  int a0;
  int a1;
  int a2;
  int temp;

  va_start(ap, fmt); /* make ap point to 1st unnamed arg */
  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }
    a0 = 0;
    a1 = a2 = -2;
    if (*++p == '-')
      a0 = 1;
    else
      --p;
    if (isdigit(*++p)) {
      temp = *p - '0';
      while (isdigit(*++p))
	temp = temp * 10 + *p - '0';
      a1 = temp;
      --p;
    }
    else if (*p == '*')
      a1 = -1;
    else
      --p;
    if (*++p == '.') {
      temp = 0;
      while (isdigit(*++p))
	temp = temp * 10 + *p - '0';
      a2 = temp;
      --p;
    }
    else if (*p == '*')
      a2 = -1;
    else
      --p;
    switch (*++p) {
    case 'd':
      if (!a0) {
	if (a1 == -2 && a2 == -2) {
	  ival = va_arg(ap, int);
	  printf("%d", ival);
	}
	else if (a1 == -2) {
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%.*d", a2, ival);
	}
	else if (a2 == -2) {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%*d", a1, ival);
	}
	else {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%*.*d", a1, a2, ival);
	}
      }
      else {
	if (a1 == -2 && a2 == -2) {
	  ival = va_arg(ap, int);
	  printf("%-d", ival);
	}
	else if (a1 == -2) {
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%-.*d", a2, ival);
	}
	else if (a2 == -2) {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%-*d", a1, ival);
	}
	else {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  ival = va_arg(ap, int);
	  printf("%-*.*d", a1, a2, ival);
	}
      }
      break;
    case 'f':
      if (!a0) {
	if (a1 == -2 && a2 == -2) {
	  dval = va_arg(ap, double);
	  printf("%f", dval);
	}
	else if (a1 == -2) {
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%.*f", a2, dval);
	}
	else if (a2 == -2) {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%*f", a1, dval);
	}
	else {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%*.*f", a1, a2, dval);
	}
      }
      else {
	if (a1 == -2 && a2 == -2) {
	  dval = va_arg(ap, double);
	  printf("%-f", dval);
	}
	else if (a1 == -2) {
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%-.*f", a2, dval);
	}
	else if (a2 == -2) {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%-*f", a1, dval);
	}
	else {
	  if (a1 == -1)
	    a1 = va_arg(ap, int);
	  if (a2 == -1)
	    a2 = va_arg(ap, int);
	  dval = va_arg(ap, double);
	  printf("%-*.*f", a1, a2, dval);
	}
      }
      break;
    case 's':
      for (sval = va_arg(ap, char *); *sval; sval++)
	putchar(*sval);
      break;
    default:
      putchar(*p);
      break;
    }
  }
  va_end(ap); /* clean up when done */
}
