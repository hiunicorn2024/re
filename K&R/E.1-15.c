#include <stdio.h>

#define LOWER 0
#define UPPER 100
#define STEP 10

float ftoc(int fahr);

main()
{
  int fahr;

  for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
    printf("%3d\t%10.2f\n", fahr, ftoc(fahr));
}

float ftoc(int fahr)
{
  return 5.0 / 9.0 * (fahr - 32);
}
