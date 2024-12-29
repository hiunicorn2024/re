#include <stdio.h>
#include <string.h>

/* find:  print lines that match pattern from 1st arg */
main(int argc, char *argv[])
{
  char line[1000];
  int found = 0;
  FILE *a;

  if (argc < 2)
    printf("ERROR:\nneed command line arguments:\nfind_pattern name_of_file1 name_of_file2 ..."
	   "(read from the set of files)\nor\nfind_pattern (read from stdin)\n");
  else if (argc > 2) {
    int i = 1;
    while (argv[++i] != NULL) {
      if ((a = fopen(argv[i], "r")) == NULL)
	fprintf(stderr, "%s: can't open %s\n", argv[0], argv[i]);
      else {
	printf("IN \"%s\":\n", argv[i]); 
	while (fgets(line, 1000, a) != NULL)
	  if (strstr(line, argv[1]) != NULL) {
	    printf("%s", line);
	    found++;
	  }
	fclose(a);
      }
    }
  }
  else
    while (fgets(line, 1000, stdin) != NULL)
      if (strstr(line, argv[1]) != NULL) {
	printf("%s", line);
	found++;
      }
  return found;
}
