#include <string.h>

#include "isprime.h"
#include "unity.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_isprime_should_return_true_for_primes(void)
{
  TEST_ASSERT_TRUE(is_prime(31LL, 10));
  TEST_ASSERT_TRUE(is_prime(11LL, 10));
}

void test_isprime_should_return_false_for_non_primes(void)
{
  TEST_ASSERT_FALSE(is_prime(30LL, 10));
  TEST_ASSERT_FALSE(is_prime(27LL, 10));
}

// not needed when using generate_test_runner.rb
int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_isprime_should_return_true_for_primes);
  RUN_TEST(test_isprime_should_return_false_for_non_primes);
  return UNITY_END();
}