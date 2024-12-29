#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN  100

enum { NAME, PARENS, BRACKETS, DATATYPE };

void dcl(int infunc);
void dirdcl(void);
void undcl(void);

int gettoken(void);

int tokentype;           /* type of last token */
char token[MAXTOKEN];    /* last token string */
char out[1000];
char out2[1000];
char error[1000];

int main(int argc, char **argv)  /* convert declaration to words */
{
  if (argc == 2 && 
      (strcmp(*(argv + 1), "dcl") == 0 || 
       strcmp(*(argv + 1), "undcl") == 0)) {
    if (strcmp(*(argv + 1), "undcl") == 0) {
      error[0] = '\0';
      out2[0] = '\0';
      undcl();
      printf("%s\n", out2);
    }
    else {
      out[0] = '\0';
      error[0] = '\0';
      dcl(1);
      if (tokentype != '\n') {
	strcat(error, "ERROR: error syntax\n");
      }
      if (error[0])
	printf("%s", error);
      else
	printf("%s\n", out);
    }
    system("PAUSE");
    return 0;
  }
  else
    printf("need one order line argument: \"dcl\" or \"undcl\"\n");
}

/* dcl:  parse a declarator */
void dcl(int infunc)
{
  char datatype[MAXTOKEN]; /* data type = char, int, etc. */
  int ns;
  ;
 dcl0:
  if (infunc == 1) {
    if (gettoken() != DATATYPE) {
      strcat(error, "ERROR: error datatype\n");
      return;
    }
    strcpy(datatype, token);
  }
  else
    datatype[0] = '\0';
  for (ns = 0; gettoken() == '*'; )    /* count *'s */
    ns++;
  dirdcl();
  while (ns-- > 0)
    strcat(out, " pointer to");
  strcat(out, " ");
  strcat(out, datatype);
  if (tokentype == ',') {
    strcat(out, ", ");
    goto dcl0;
  }
}

/* dirdcl:  parse a direct declarator */
void dirdcl(void)
{
  int type;
  int func_state = 0;
  int brkt_state = 0;

  if (tokentype == '(') {         /* ( dcl ) */    
    dcl(0);
    if (tokentype != ')')
      strcat(error, "ERROR: missing )\n");
  }
  else if (tokentype == NAME) {   /* variable name */
    strcat(out, token);
    strcat(out, ":");
  }
  else {
    strcat(out, "void: ");
    type = tokentype;
    goto dirdcl0;
  }
  while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(') {
  dirdcl0:
    if (type == PARENS) {
      if (brkt_state)
	strcat(error, "ERROR: function returning array\n");
      func_state = 1;
      strcat(out, " function (void) returning");
    }
    else if (type == BRACKETS) {
      if (func_state)
	strcat(error, "ERROR: function declared as array\n");
      brkt_state = 1;
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
    else if (type == '(') {
      strcat(out, " function (");
      dcl(1);
      strcat(out, ") returning");
      if (tokentype != ')')
	strcat(error, "ERROR: missing )\n");
    }
    else
      break;
  }
}

/* undcl:  convert word descriptions to declarations */
void undcl(void)
{
  int type;
  char temp[MAXTOKEN];

  while ((type = gettoken()) != '\n') {
    //    strcat(out2, token);
  undcl0:
    if (type == PARENS || type == BRACKETS)
      strcat(out2, token);
    else if (type == '*') {
      if ((type = gettoken()) == BRACKETS)
	sprintf(temp, "(*%s)", out2);
      else
	sprintf(temp, "*%s", out2);
      strcpy(out2, temp);
      goto undcl0;
    }
    else if (type == NAME) {
      if (out2[0] != '\0') {
	sprintf(temp, "%s %s", token, out2);
	strcpy(out2, temp);
      }
      else
	strcpy(out2, token);
    }
    else if (type == DATATYPE) {
      sprintf(temp, "%s %s", token, out2);
      strcpy(out2, temp);
    }
    else if (type == '\n')
      break;
    else
      printf("invalid input at %s\n", token);
  }
}

int gettoken(void)  /* return next token */
{
  int c, getch(void);
  void ungetch(int);
  char *p = token;

  while ((c = getch()) == ' ' || c == '\t')
    ;
  if (c == '(') {
    if ((c = getch()) == ')') {
      strcpy(token, "()");
      return tokentype = PARENS;
    } 
    else if (c == 'v') {
      if ((c = getch()) == 'o' && (c = getch()) == 'i' && (c = getch()) == 'd' &&
	  (c = getch()) == ')') {
	strcpy(token, "(void)");
	return tokentype = PARENS;
      }
    }
    else {
      ungetch(c);
      return tokentype = '(';
    }
  } 
  else if (c == '[') {
    for (*p++ = c; (*p++ = getch()) != ']'; )
      ;
    *p = '\0';
    return tokentype = BRACKETS;
  }
  else if (isalpha(c)) {
    char *p_bk = p;
    int pre_state = 0;
 
   for (*p++ = c; isalnum(c = getch()) || c == '_'; )
      *p++ = c;
    *p = '\0';
    if (strcmp(p_bk, "static") == 0 || strcmp(p_bk, "const") == 0) {
      pre_state = 1;
      *p++ = ' ';
      p_bk = p;
      if (isalpha(c = getch())) {
	for (*p++ = c; isalnum(c = getch()) || c == '_'; )
	  *p++ = c;
	*p = '\0';
	goto s0;
      }
      else
	strcat(error, "ERROR: uncompleted datatype\n");
    }
  s0:
    if (strcmp(p_bk, "void") == 0) {
      ungetch(c);
      return tokentype = DATATYPE;
    }
    if (strcmp(p_bk, "long") == 0 || strcmp(p_bk, "short") == 0) {
      pre_state = 1;
      *p++ = ' ';
      p_bk = p;
      if (isalpha(c = getch())) {
	for (*p++ = c; isalnum(c = getch()) || c == '_'; )
	  *p++ = c;
	*p = '\0';
	goto s1;
      }
      else
	strcat(error, "ERROR: uncompleted datatype\n");
    }
  s1:
    if (strcmp(p_bk, "unsigned") == 0) {
      pre_state = 1;
      *p++ = ' ';
      p_bk = p;
      if (isalpha(c = getch())) {
	for (*p++ = c; isalnum(c = getch()) || c == '_'; )
	  *p++ = c;
	*p = '\0';
	goto s2;
      }
      else
	strcat(error, "ERROR: uncompleted datatype\n");
    }
  s2:
    if (strcmp(p_bk, "double") == 0 || strcmp(p_bk, "int") == 0 ||
	strcmp(p_bk, "char") == 0 || strcmp(p_bk, "float") == 0) {
      ungetch(c);
      return tokentype = DATATYPE;
    }
    if (pre_state) {
      strcat(error, "ERROR: uncompleted datatype\n");
      return;
    }
    else {
      ungetch(c);
      return tokentype = NAME;
    }
  }
  else
    return tokentype = c;
}

#define SIZE_buff 1000

int buff[SIZE_buff];
int buffp = 0;

int getch(void);
void ungetch(int);

int getch(void) {
  return buffp > 0 ? buff[--buffp] : getchar();
}

void ungetch(int m) {
  if (buffp >= SIZE_buff)
    printf("ERROR in ungetch: too many elements have been in stack\n");
  else
    buff[buffp++] = m;
}
