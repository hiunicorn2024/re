#include <stdio.h>

#define SIZE_buff 1000

int buff[SIZE_buff];
int buffp = 0;

int getch(void);
void ungetch(int);

int getch(void)
{
  return buffp > 0 ? buff[--buffp] : getchar();
}

void ungetch(int m)
{
  if (buffp >= SIZE_buff)
    printf("ERROR in ungetch: too many elements have been in stack\n");
  else
    buff[buffp++] = m;
}
