#include <stdio.h>

void reverse(char s[]);

main()
{
  char m[] = "12345abcde";

  printf("BEFORE:\n%s\n", m);
  reverse(m);
  printf("AFTER:\n%s\n", m);
}

void reverse(char s[])
{
  static int i = 0, k, temp;

  if (s[i * 2 + 1] != '\0' && s[i * 2 + 2] != '\0') {
    ++i;
    reverse(s);
  }
  else
    k = s[i * 2 + 1] == '\0' ? i : i + 1;
  temp = s[i];
  s[i--] = s[k];
  s[k++] = temp;
  if (i == -1)
    i = 0;
}
