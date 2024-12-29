#include <stdio.h>

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

void f(char **);

main()
{
  int a = 0, b = 0;
  static char daytab[2][13];
  f(daytab);
  return;
  printf("%d\n", day_of_year(2012, 13, 1));
  month_day(2012, 366, &a, &b);
  printf("%d\n%d\n", a, b);
}

static char daytab[2][13] = {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

void f(char (*m)[13]) {
  printf("%d\n", **m);
}

/* day_of_year:  set day of year from month & day */
int day_of_year(int year, int month, int day)
{
  int i, leap;
  leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
  if (month < 0 || month > 12) {
    printf("the month number is beyond the limit.\n");
    return -1;
  }
  if (day <= 0 || day > daytab[leap][month]) {
    printf("the day number is beyond the limit.\n");
    return -1;
  }
  for (i = 1; i < month; i++)
    day += daytab[leap][i];
  return day;
}

/* month_day:  set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
  int i, leap, x = 0;
  
  leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
  for (i = 12; i;)
    x += daytab[leap][i--];
  if (yearday <= 0 || yearday > x) {
    printf("the yearday number is beyond the limit.\n");
    return;
  }
  for (i = 1; yearday > daytab[leap][i]; i++)
    yearday -= daytab[leap][i];
  *pmonth = i;
  *pday = yearday;
}
