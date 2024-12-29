#include <stdio.h>

#define MAX 1000

int readline(char s[], int lim);
void expand(char a[], char b[]);

main()
{
  char a[MAX], b[MAX];

  while (readline(a, MAX) > 0) {
    expand(a, b);
    printf("%s", b);
  }
}

int readline(char s[], int lim) {
  int c, i = 0;

  while (i + 1 < lim && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

void expand(char a[], char b[]) {
  char c;
  int i, j, n, k, state;

  i = j = 0;
  while ((c = a[i]) != '\0') {
    k = state = 0;
    if (c <'Z' && c >= 'A') {
      b[j++] = c;
      while (state == 0 && a[i + (++k)] == '-') {
	if (a[i + (++k)] <= 'Z' && a[i + k] > c) {
	  for (n = 0; n < a[i + k] - c - 1; ++n)
	    b[j++] = c + n + 1;
	  c = b[j++] = a[i + k];
	}
	else {
	  i += k - 1;
	  state = 1;
	}
      }
      if (state == 0)
	i += k;
    }
    else if (c <= 'z' && c >= 'a') {
      b[j++] = c;
      while (state == 0 && a[i + (++k)] == '-') {
	if (a[i + (++k)] <= 'z' && a[i + k] > c) {
	  for (n = 0; n < a[i + k] - c - 1; ++n)
	    b[j++] = c + n + 1;
	  c = b[j++] = a[i + k];
	}
	else {
	  i += k - 1;
	  state = 1;
	}
      }
      if (state == 0)
	i += k;     
    }
    else if (c <= '9' && c >= '0') {
      b[j++] = c;
      while (state == 0 && a[i + (++k)] == '-') {
	if (a[i + (++k)] <= '9' && a[i + k] > c) {
	  for (n = 0; n < a[i + k] - c - 1; ++n)
	    b[j++] = c + n + 1;
	  c = b[j++] = a[i + k];
	}
	else {
	  i += k - 1;
	  state = 1;
	}
      }
      if (state == 0)
	i += k;
    }
    else {
      b[j++] = c;
      ++i;
    }
  }
  b[j] = '\0';
}
