#include <stdio.h>

main()
{
  int i, j, k, c;
  int n_digit, n_letter, n_other;

  n_digit = n_letter = n_other = 0;
  while ((c = getchar()) != EOF) {
    if (c <= 'Z' && c >= 'A' || c <= 'z' && c >= 'a')
      ++n_letter;
    else if (c <= '9' && c >= '0')
      ++n_digit;
    else
      ++n_other;
  }
  printf(">=");
  for (i = 10; i >= 0; --i) {
    printf("\t   %3d|", i + 1);
    if (n_digit >= i + 1)
      printf("*******");
    else
      printf("       ");
    if (n_letter >= i + 1)
      printf("*******");
    else
      printf("       ");
    if (n_other >= i + 1)
      printf("*******\n");
    else
      printf("       \n");    
  }
  printf("\t------------------------------\n"
	 "\t        digit  letter other\n"); 
}
