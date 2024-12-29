#include <stdio.h>

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

main()
{
  int a = 0, b = 0;

  printf("%d\n", day_of_year(2012, 10, 1));
  month_day(2012, 275, &a, &b);
  printf("%d %d\n", a, b);
}

static char *daytab[2] = {
  "\0\37\34\37\36\37\36\37\37\36\37\36\37",
  "\0\37\35\37\36\37\36\37\37\36\37\36\37"
};

/* day_of_year:  set day of year from month & day */
int day_of_year(int year, int month, int day)
{
  int i, leap;
  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  if (month < 0 || month > 12) {
    printf("the month number is beyond the limit.\n");
    return -1;
  }
  if (day <= 0 || day > *(*(daytab + leap) + month)) {
    printf("the day number is beyond the limit.\n");
    return -1;
  }
  for (i = 1; i < month; i++)
    day += *(*(daytab + leap) + i);
  return day;
}

/* month_day:  set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
  int i, leap, x = 0;
  
  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  for (i = 12; i;)
    x += *(*(daytab + leap) + (i--));
  if (yearday <= 0 || yearday > x) {
    printf("the yearday number is beyond the limit.\n");
    return;
  }
  for (i = 1; yearday > *(*(daytab + leap) + i); i++)
    yearday -= *(*(daytab + leap) + i);
  *pmonth = i;
  *pday = yearday;
}
