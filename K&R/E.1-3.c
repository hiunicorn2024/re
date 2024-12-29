#include <stdio.h>

main()
{
  int step, upper, lower;
  float celsius, fahr;

  upper = 100;
  step = 20;
  lower = 0;
  fahr = 100;
  printf("********************** a simple Fahrenheit-Celsius table **********************\nFahrenheit         Celsius\n");
  while (fahr >= lower) {
    celsius = 5.0 / 9.0 * (fahr - 32);
    printf("%10.0f\t%10.2f\n", fahr, celsius);
    fahr = fahr - step;
  }
}
