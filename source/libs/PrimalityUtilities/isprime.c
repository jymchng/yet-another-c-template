/* Codes copied from Effective C book*/

#include "isprime.h"

#include <stdbool.h>
#include <stdlib.h>

/**
 * Computes (x^y) % p efficiently using modular exponentiation.
 *
 * @param x The base number.
 * @param y The exponent.
 * @param p The modulus.
 * @return The result of (x^y) % p.
 */
static unsigned long long power(unsigned long long x,
                                unsigned long long y,
                                unsigned long long p)
{
  unsigned long long result = 1;
  x %= p;
  while (y) {
    if (y & 1)
      result = (result * x) % p;
    y >>= 1;
    x = (x * x) % p;
  }
  return result;
}

/**
 * Performs the Miller-Rabin primality test to check if 'n' is likely to be
 * prime.
 *
 * @param d The odd number derived from (n - 1).
 * @param n The number to test for primality.
 * @return True if 'n' is likely to be prime, false otherwise.
 */
static bool miller_rabin_test(unsigned long long d, unsigned long long n)
{
  unsigned long long a = 2 + (unsigned long long)rand() % (n - 4);
  unsigned long long x = power(a, d, n);
  if (x == 1 || x == n - 1)
    return true;
  while (d != n - 1) {
    x = (x * x) % n;
    d *= 2;
    if (x == 1)
      return false;
    if (x == n - 1)
      return true;
  }
  return false;
}

/**
 * Checks if a given number 'n' is prime using the Miller-Rabin primality test.
 *
 * @param n The number to check for primality.
 * @param k The number of iterations for the Miller-Rabin test.
 * @return True if 'n' is prime, false otherwise.
 */
bool is_prime(unsigned long long n, unsigned int k)
{
  if (n <= 1 || n == 4)
    return false;
  if (n <= 3)
    return true;
  unsigned long long d = n - 1;
  while (d % 2 == 0)
    d /= 2;
  for (; k != 0; --k) {
    if (!miller_rabin_test(d, n))
      return false;
  }
  return true;
}