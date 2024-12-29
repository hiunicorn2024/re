#include <stdio.h>
#include <string.h>

int fcmp(char *a, char *b);

int main(void)
{
  char *a, *b;
  a = "/home/hiunicorn/Documents/0.txt";
  b = "/home/hiunicorn/Documents/1.txt";
  fcmp(a, b);
  return 0;
}

int fcmp(char *f1, char *f2) {
  FILE *fp1, *fp2;
  char line1[100], line2[100];
  char *a, *b;
  fp1 = fopen(f1, "r");
  fp2 = fopen(f2, "r");
 fcmp0:
  a = fgets(line1, 100, fp1);
  b = fgets(line2, 100, fp2);
  if (a == NULL && b == NULL)
    return 0;
  else if (a == NULL || b == NULL) {
    if (a == NULL)
      printf("(none)\n%s", line2);
    else
      printf("%s(none)\n", line1);
    return 1;
  }
  else
    if (strcmp(line1, line2) == 0)
      goto fcmp0;
    else {
      printf("%s%s", line1, line2);
      return 1;
    }
}
