#ifndef GCD_H
#define GCD_H

/**
 * @brief Calculates the greatest common divisor (GCD) of two integers.
 *
 * This function implements the Euclidean algorithm to compute the greatest
 * common divisor (GCD) of two given integers. The Euclidean algorithm is based
 * on the principle that the GCD of two numbers does not change if the larger
 * number is replaced by its difference with the smaller number.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @return The greatest common divisor of the two integers.
 */
int gcd(int a, int b);

#endif  // GCD_H
