#include "gcd.h"

// Function to calculate the GCD of two numsecond_inters using Euclidean
// algorithm
int gcd(int first_int, int second_int)
{
  while (second_int != 0) {
    int temp = second_int;
    second_int = first_int % second_int;
    first_int = temp;
  }
  return first_int;
}
