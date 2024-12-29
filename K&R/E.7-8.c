#include <stdio.h>

int main(int argc, char **argv)
{
  char s[1000];
  FILE *fp;
  int i = 1;

  if (argc == 1) {
    while (fgets(s, 1000, stdin) != NULL)
      fputs(s, stdout);
  }
  else {
    do {      
      if ((fp = fopen(argv[i], "r")) == NULL)
	fprintf(stderr, "%s: can't open %s\n", argv[0], argv[i]);
      else {
	printf("****************************************"
	       "****************************************\n"
	       "\t\t\t\tPAGE %d: \"%s\"\n"
	       "****************************************"
	       "****************************************\n",
	       i, argv[i]);
	while (fgets(s, 1000, fp) != NULL)
	  fputs(s, stdout);
      }
    } while (argv[++i] != NULL);
  }
  return 0;
}
