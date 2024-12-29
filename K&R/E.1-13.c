#include <stdio.h>

#define MAX_LENGTH 12
#define MAX_NUM 12
#define IN 1
#define OUT 0

main()
{
  int state, c, word_length, i, n;
  int a[MAX_LENGTH + 1];

  state = OUT;
  word_length = 0;
  for (i = 0; i <= MAX_LENGTH; ++i)
    a[i] = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\t' || c == '\n') {
      if (state == IN) {
	state = OUT;
	if (word_length > MAX_LENGTH)
	  word_length = MAX_LENGTH + 1;
	++a[word_length - 1];
	word_length = 0;
      }
    }
    else {
      if (state == OUT)
	state = IN;
      ++word_length;
    }
  }
  for (i = 0; i <= MAX_LENGTH; ++i)
    if (a[i] > MAX_NUM)
      a[i] = MAX_NUM + 1;
  printf(">=");
  for (i = MAX_NUM + 1; i >= 1; --i) {
    printf("\t  %6d|", i);
    for (n = 0; n <= MAX_LENGTH; ++n) {
      if (a[n] >= i)
	printf(" ** ");
      else
	printf("    ");
    }
    printf("\n");
  }
  printf("\t        |");
  for (i = 0; i <= MAX_LENGTH; ++i)
    printf("----");
  printf("\n\t         ");
  for (i = 1; i <= MAX_LENGTH; ++i)
    printf(" %2d ", i);
  printf(">%2d \n", MAX_LENGTH);
}
