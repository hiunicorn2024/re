#include <stdio.h>
#include <ctype.h>
#include <string.h>

int linelen = 80;
int tablen = 8;
int tabcolumns = 1;
int tabcolumns1 = 80;

#define TABLEN(j) (linelen - j % linelen <= tablen ? tablen - 2 - j % tablen : tablen - 1 - j % tablen) + 1

int readline(char *line, int max);

main(int argc, char **argv)
{
  int c, lim = 1000;
  char s[lim + 1];
  char a[lim + 1];
  int mode_state = 0; /* value 1 is for entab and value 2 is for detab */
  int i = 0;
  char *p;
  
  while (--argc) {
    p = *(++argv);
    if (!strcmp(p, "entab"))
      mode_state = 1;
    else if (!strcmp(p, "detab"))
      mode_state = 2;
    else if (*p == '<') {
      ++p;
      int x = 0;
      while (isdigit(*p))
	x = x * 10 + *p++ - '0';
      if (x > 0)
	linelen = x;
    }
    else if (*p == '>') {
      ++p;
      int x = 0;
      while (isdigit(*p))
	x = x * 10 + *p++ - '0';
      if (x > 0)
	tablen = x;
    }
    else if (*p == '-') {
      ++p;
      int x = 0;
      while (isdigit(*p))
	x = x * 10 + *p++ - '0';
      if (x > 0)
	tabcolumns = x;
    }
    else if (*p == '+') {
      ++p;
      int x = 0;
      while (isdigit(*p))
	x = x * 10 + *p++ - '0';
      if (x > 0)
	tabcolumns1 = x;
    } 
    else
      ;
  }

  switch (mode_state) {
  case 1:
    goto entab;
  case 2:
    goto detab;
  default:
    goto end;
  }

 entab:
  while (readline(s, lim) > 0) {
    int i = 0;     /* 读已输入的行 */
    int j = 0;     /* 读转换后的行 */
    int p_a = 0;   /* p_a定位 */
    int spnum = 0; /* 空格数 */
    int m, k;      /* 计数 */
    do {
      if (*(s + i) == ' ' && (p_a % linelen) <= tabcolumns1 - 1 && (p_a % linelen) >= tabcolumns - 1)
	++spnum;
      else {
	switch (spnum) {
	case 0:
	  ;
	  break;
	default:
	  {
	    int p_s = p_a + spnum;
	    while (p_a + TABLEN((p_a % linelen)) <= p_s) {
	      if (TABLEN((p_a % linelen))) {
		*(a + j++) = '\t';
		p_a += TABLEN((p_a % linelen));
	      }
	      else {
		*(a + j++) = '\n';
		++p_a;
	      }
	    }
	    while ((p_a + 1) <= p_s) {
	      *(a + j++) = ' ';
	      ++p_a;
	    }
	  }	  /* 填空格 */
	  break;
	}
	switch (*(s + i)) {
	case '\t':
	  *(a + j++) = *(s + i);
	  p_a += TABLEN((p_a % linelen)); /* 上TAB */
	  break;
	default:
	  *(a + j++) = *(s + i);
	  ++p_a;
	  break;
	}
	spnum = 0;
      }
    } while (*(s + i++) != '\0');
    printf ("%s", a);
  }
  return;
  
 detab:
  while (readline(s, lim) > 0) {
    int j = 0, i = 0, m, k, p_a = 0;
    do {
      if (p_a > linelen - 1)
	p_a -= linelen - 1;
      if (*(s + i) == '\t' && p_a >= tabcolumns - 1 && p_a <= tabcolumns1 - 1) {
	k = TABLEN(p_a);
	while (k--) {
	  *(a + j++) = ' ';
	  ++p_a;
	}
      }
      else {
	*(a + j++) = *(s + i);
	if (*(s + i) == '\t')
	  p_a += TABLEN(p_a);
	else
	  ++p_a;
      }
    } while (*(s + i++) != '\0');   
    printf ("%s", a);
  }
 end:
  return;
}

int readline(char *line, int max) {
  int c;
  char *s = line;

  while (--max && (c = getchar()) != EOF && c != '\n')
    *line++ = c;
  if (c == '\n')
    *line++ = c;
  *line = '\0';
  return line - s;
}
