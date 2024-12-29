#include <stdio.h>
#include <time.h>

int binsearch(int x, int v[], int n);
int binsearch2(int x, int v[], int n);

main()
{
  int a[10000];
  int i, test;
  clock_t time;

  for (i = 0; i < 10000; ++i)
    a[i] = i;
  time = clock();
  for (i = 0; i < 1000000; ++i)
    test = binsearch(5000, a, 10000);
  time = clock() - time;
  printf("binsearch1 took %ld clocks\n", time);
  time = clock();
  for (i = 0; i < 1000000; ++i)
    test = binsearch2(5000, a, 10000);
  time = clock() - time;
  printf("binsearch2 took %ld clocks\n", time);
}

int binsearch(int x, int v[], int n) {
  int low, high, mid;  

  low = 0;
  high = n - 1;
  while (low <= high) {
    mid = (low + high) / 2;
    if (x < v[mid])
      high = mid - 1;
    else if (x > v[mid])
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}

int binsearch2(int x, int v[], int n) {
  int low, high, mid;  

  low = 0;
  high = n - 1;
  mid = (low + high) / 2;
  while (low <= high && x != v[mid]) {
    if (x < v[mid])
      high = mid - 1;
    else
      low = mid + 1;
    mid = (low + high) / 2;
  }
  if (v[mid] == x)
    return mid;
  return -1;
}
