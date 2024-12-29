#include <stdio.h>

#define MAX 1000

int readline(char s[], int lim);
void escape(char s[], char t[]);
void re_escape(char s[], char t[]);

main()
{
  char a[MAX], b[MAX];

  if (readline(a, MAX) > 0) {
    escape(b, a);
    printf("%s", b);
    putchar('\n');
  }
  if (readline(a, MAX) > 0) {
    re_escape(b, a);
    printf("%s", b);
  }
}

int readline(char s[], int lim) {
  int c, i = 0;

  while (i < lim - 1 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = '\n';
  s[i] = '\0';
  return i;
}

void escape(char s[], char t[]) {
  int i = 0, j = 0;
  while (t[i] != '\0') {
    switch(t[i]) {
    case '\n':
      s[j++] = '\\';
      s[j++] = 'n';
      break;
    case '\t':
      s[j++] = '\\';
      s[j++] = 't';
      break;
    default:
      s[j++] = t[i];
      break;
    }
    ++i;
  }
  s[j] = '\0';
}

void re_escape(char s[], char t[]) {
  int i = 0, j = 0;

  while (t[i] != '\0') {
    switch(t[i]) {
    case '\\':
      switch(t[i + 1]) {
      case't':
	s[j++] = '\t';
	++i;
	break;
      case'n':
	s[j++] = '\n';
	++i;
	break;
      default:
	s[j++] = '\\';
	break;
      }
      break;
    default:
      s[j++] = t[i];
      break;
    }
    ++i;
  }
  s[j] = '\0';
}
