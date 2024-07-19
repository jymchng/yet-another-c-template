/* Codes copied from Effective C book*/

#ifndef PRIMETEST_IS_PRIME_H
#define PRIMETEST_IS_PRIME_H

#include <stdbool.h>

/**
 * @brief Determines if a number is prime using the Miller-Rabin primality test.
 *
 * This function checks if the given number `n` is a prime number using the
 * Miller-Rabin primality test. The Miller-Rabin test is a probabilistic
 * algorithm that determines whether a given number is likely to be prime.
 * It is highly efficient and suitable for testing large numbers.
 *
 * The parameter `k` specifies the number of iterations for the test. A higher
 * value of `k` increases the accuracy of the test but also increases the
 * computational cost. Generally, values of `k` between 20 and 50 are
 * recommended for a good balance between accuracy and performance.
 *
 * @param n The number to be tested for primality. Must be a positive integer.
 * @param k The number of iterations to perform in the Miller-Rabin test.
 *          Higher values result in more accurate results.
 * @return `true` if the number is probably prime, `false` if the number is
 *         composite.
 *
 * @note The Miller-Rabin test is a probabilistic test, meaning that it can
 *       occasionally produce false positives (indicating that a composite
 *       number is prime). However, the probability of such errors decreases
 *       exponentially with the number of iterations `k`.
 *
 * @note This function is intended for use with unsigned long long integers.
 *
 * @warning The function may not behave correctly if `n` is 0 or 1, as these
 *          values are not prime by definition. It is recommended to handle
 *          such edge cases separately in your code.
 *
 * @see https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test for
 *      more information on the Miller-Rabin primality test.
 */
bool is_prime(unsigned long long n, unsigned k);

#endif  // PRIMETEST_IS_PRIME_H
