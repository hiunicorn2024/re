#include <stdio.h>

#define swap(t, x, y)				\
  {						\
    t temp;					\
    temp = x;					\
    x = y;					\
    y = temp;					\
  }

main()
{
  int a = 20, b = 10;

  printf("BEFORE:\ta = %d, b = %d\n", a, b);
  swap(int, a, b);
  printf("AFTER:\ta = %d, b = %d\n", a, b);
}
