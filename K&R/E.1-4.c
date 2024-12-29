#include <stdio.h>

main()
{
  int step, upper, lower;
  float celsius, fahr;
  upper = 60;
  step = 20;
  lower = -40;
  
  celsius = lower;
  while (celsius <= upper) {
    fahr = 9.0 / 5.0 * celsius + 32;
    printf("%3.0f\t%10.0f\n", celsius, fahr);
    celsius = celsius + step;
  }
}
