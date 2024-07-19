#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include <inttypes.h>

#include "unity.h"

// Must always #define VECTOR_H_i_TYPE first before including "vector.h"
#define VECTOR_H_i_TYPE uint16_t
#include "vector.h"

// Mock the print function to avoid output during tests
void print_uint16_t(uint16_t elem)
{
  printf("%d", elem);
}

// Setup the test environment
void setUp(void)
{
  // Nothing to set up
}

// Teardown the test environment
void tearDown(void)
{
  // Nothing to clean up
}

/**
 * @brief Test vector creation with default constructor.
 */
void test_vector_new(void)
{
  Vector* v = vector_new();
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_EQUAL_INT(VECTOR_INIT_CAPACITY, v->capacity);
  TEST_ASSERT_EQUAL_INT(0, v->length);
  vector_free(v);
}

/**
 * @brief Test vector push and length.
 */
void test_vector_push(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);
  vector_push(v, 4);
  vector_push(v, 5);  // Triggers reallocation

  TEST_ASSERT_EQUAL_INT(5, v->length);
  TEST_ASSERT_EQUAL_INT(8, v->capacity);  // After reallocation

  TEST_ASSERT_EQUAL_UINT16(1, v->data[0]);
  TEST_ASSERT_EQUAL_UINT16(2, v->data[1]);
  TEST_ASSERT_EQUAL_UINT16(3, v->data[2]);
  TEST_ASSERT_EQUAL_UINT16(4, v->data[3]);
  TEST_ASSERT_EQUAL_UINT16(5, v->data[4]);

  vector_free(v);
}

/**
 * @brief Test vector pop operation.
 */
void test_vector_pop(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  uint16_t* elem = vector_pop(v);
  TEST_ASSERT_EQUAL_UINT16(3, *elem);
  TEST_ASSERT_EQUAL_INT(2, v->length);

  elem = vector_pop(v);
  TEST_ASSERT_EQUAL_UINT16(2, *elem);
  TEST_ASSERT_EQUAL_INT(1, v->length);

  elem = vector_pop(v);
  TEST_ASSERT_EQUAL_UINT16(1, *elem);
  TEST_ASSERT_EQUAL_INT(0, v->length);

  // Pop from an empty vector
  elem = vector_pop(v);
  TEST_ASSERT_EQUAL_PTR(NULL,
                        elem);  // NULL should be represented by 0 in this case

  vector_free(v);
}

/**
 * @brief Test vector get and set operations.
 */
void test_vector_get_and_set(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  uint16_t* elem = vector_get_mut(v, 1);
  TEST_ASSERT_NOT_NULL(elem);
  *elem = 99;

  const uint16_t* elem_const = vector_get(v, 1);
  TEST_ASSERT_NOT_NULL(elem_const);
  TEST_ASSERT_EQUAL_UINT16(99, *elem_const);

  vector_set(v, 2, 55);
  TEST_ASSERT_EQUAL_UINT16(55, v->data[2]);

  vector_free(v);
}

/**
 * @brief Test vector find operation.
 */
void test_vector_find(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  int found;
  VECTOR_H_i_TYPE to_find = 2;
  VECTOR_H_i_TYPE* found_element = vector_find(v, to_find, &found);
  TEST_ASSERT_EQUAL_INT(1, found);
  TEST_ASSERT_EQUAL_UINT16(2, *found_element);

  VECTOR_H_i_TYPE to_find_two = 4;
  found_element = vector_find(v, to_find_two, &found);
  TEST_ASSERT_EQUAL_INT(0, found);

  vector_free(v);
}

/**
 * @brief Test vector reserve operation.
 */
void test_vector_reserve(void)
{
  Vector* v = vector_new();
  vector_reserve(v, 10);  // Reserve more space than the current capacity

  TEST_ASSERT_EQUAL_INT(16, v->capacity);
  for (VECTOR_H_i_TYPE i = 0; i < 16; i++) {
    vector_push(v, i);
    TEST_ASSERT_EQUAL_INT(1 + i, v->length);
    TEST_ASSERT_EQUAL_UINT16(i, v->data[i]);
    TEST_ASSERT_EQUAL_INT(16, v->capacity);  // Capacity stays the same
  }

  // Now pushing the 17-th item should make the capacity increases
  vector_push(v, 69);
  TEST_ASSERT_EQUAL_INT(17, v->length);
  TEST_ASSERT_EQUAL_INT(32, v->capacity);  // 16 * 2 = 32
  TEST_ASSERT_EQUAL_UINT16(69, v->data[16]);

  vector_free(v);
}

/**
 * @brief Test vector length and capacity after multiple operations.
 */
void test_vector_length_and_capacity(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  TEST_ASSERT_EQUAL_INT(3, v->length);
  TEST_ASSERT_EQUAL_INT(4, v->capacity);

  vector_push(v, 4);
  vector_push(v, 5);
  vector_push(v, 6);  // Triggers reallocation

  TEST_ASSERT_EQUAL_INT(6, v->length);
  TEST_ASSERT_EQUAL_INT(8, v->capacity);  // After reallocation

  vector_free(v);
}

/**
 * @brief Test vector with multiple push and pop operations.
 */
void test_vector_push_and_pop(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);
  vector_push(v, 4);
  vector_push(v, 5);
  vector_push(v, 6);
  vector_push(v, 7);
  vector_push(v, 8);
  vector_push(v, 9);
  vector_push(v, 10);  // Triggers reallocation

  TEST_ASSERT_EQUAL_INT(10, v->length);
  TEST_ASSERT_EQUAL_INT(16, v->capacity);  // After reallocation

  vector_pop(v);
  vector_pop(v);

  TEST_ASSERT_EQUAL_INT(8, v->length);

  vector_free(v);
}

/**
 * @brief Test vector reserve without reallocation if enough space.
 */
void test_vector_reserve_no_realloc(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);

  vector_reserve(v, 2);  // No reallocation needed

  TEST_ASSERT_EQUAL_INT(4, v->capacity);  // Need 2 more space,

  vector_free(v);
}

/**
 * @brief Test vector reserve with exact allocation.
 */
void test_vector_reserve_exact(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);

  vector_reserve(v, 2);  // Capacity will be resized to 4

  TEST_ASSERT_EQUAL_INT(4, v->capacity);  // Should be resized to at least 4

  vector_free(v);
}

/**
 * @brief Test vector creation with custom capacity.
 */
void test_vector_with_capacity(void)
{
  Vector* v = vector_with_capacity(10);
  TEST_ASSERT_EQUAL_INT(10, v->capacity);
  TEST_ASSERT_EQUAL_INT(0, v->length);
  vector_free(
      v);  // Note: vector_free expects a pointer, so `&v` is correct here
}

/**
 * @brief Test vector free operation.
 */
void test_vector_free(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  vector_free(v);

  // After `vector_free`, v should not be accessed
}

/**
 * @brief Test vector get function returns NULL for out-of-bounds access.
 */
void test_vector_get_out_of_bounds(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  const uint16_t* elem = vector_get(v, 5);
  TEST_ASSERT_NULL(elem);

  vector_free(v);
}

/**
 * @brief Test vector get_mut function returns NULL for out-of-bounds access.
 */
void test_vector_get_mut_out_of_bounds(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  uint16_t* elem = vector_get_mut(v, 5);
  TEST_ASSERT_NULL(elem);

  vector_free(v);
}

/**
 * @brief Test vector set function for valid index.
 */
void test_vector_set(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  vector_set(v, 1, 99);
  TEST_ASSERT_EQUAL_UINT16(99, v->data[1]);

  vector_free(v);
}

/**
 * @brief Test vector set function for invalid index.
 */
void test_vector_set_out_of_bounds(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  vector_set(v, 5, 99);

  TEST_ASSERT_EQUAL_UINT16(
      3, v->data[2]);  // The data at index 2 should remain unchanged

  vector_free(v);
}

/**
 * @brief Test `vector_set` function for a valid index after multiple pushes.
 */
void test_vector_set_valid_index(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);
  vector_push(v, 4);

  vector_set(v, 2, 99);
  TEST_ASSERT_EQUAL_UINT16(99, v->data[2]);

  vector_free(v);
}

/**
 * @brief Test vector capacity after multiple push operations and reallocation.
 */
void test_vector_capacity_after_reallocation(void)
{
  Vector* v = vector_new();
  for (VECTOR_H_i_TYPE i = 0; i < 20; ++i) {
    vector_push(v, i);
  }

  TEST_ASSERT_GREATER_THAN(8,
                           v->capacity);  // After several pushes, the capacity
                                          // should have been increased
  TEST_ASSERT_EQUAL_INT(20, v->length);

  vector_free(v);
}

/**
 * @brief Test vector_pop when vector is empty.
 */
void test_vector_pop_empty(void)
{
  Vector* v = vector_new();

  uint16_t* elem = vector_pop(v);
  TEST_ASSERT_EQUAL_PTR(NULL,
                        elem);  // Pop from an empty vector should return 0

  vector_free(v);
}

/**
 * @brief Test vector_find with different element types.
 */
void test_vector_find_different_elements(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  int found;
  VECTOR_H_i_TYPE to_find = 2;
  VECTOR_H_i_TYPE* found_element = vector_find(v, to_find, &found);
  TEST_ASSERT_EQUAL_INT(1, found);
  TEST_ASSERT_EQUAL_UINT16(2, *found_element);

  VECTOR_H_i_TYPE to_find_not_exist = 99;
  found_element = vector_find(v, to_find_not_exist, &found);
  TEST_ASSERT_EQUAL_INT(0, found);

  vector_free(v);
}

/**
 * @brief Test `vector_reserve` when a large amount of space is reserved.
 */
void test_vector_reserve_large_amount(void)
{
  Vector* v = vector_new();
  vector_reserve(v, 1000);  // Request a very large reserve

  TEST_ASSERT_GREATER_THAN(
      8, v->capacity);  // Capacity should be increased from the initial 4 to at
                        // least 1000 (or the next power of 2)
  TEST_ASSERT_EQUAL_INT(0, v->length);

  vector_free(v);
}

/**
 * @brief Test `vector_reserve` when requested reserve is smaller than the
 * current capacity.
 */
void test_vector_reserve_smaller_than_current_capacity(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);
  vector_push(v, 4);  // Initial capacity was 4, so after 4 pushes, it's enough

  vector_reserve(v, 2);

  TEST_ASSERT_EQUAL_INT(8, v->capacity);

  vector_free(v);
}

/**
 * @brief Test vector's `print` function.
 */
// void test_vector_print(void)
// {
//   Vector* v = vector_new();
//   TEST_ASSERT_NOT_NULL(v);  // Ensure the vector is created successfully
//   vector_push(v, 10);
//   vector_push(v, 20);
//   vector_push(v, 30);

//   // Redirect stdout to a buffer
//   char buffer[100];
//   FILE* new_stdout = fmemopen(buffer, sizeof(buffer), "w");
//   TEST_ASSERT_NOT_NULL(
//       new_stdout);  // Ensure the new stdout is created successfully

//   vector_print(v);

//   fflush(new_stdout);
//   fclose(new_stdout);  // Properly close the file stream

//   // Check that the output buffer contains the correct string
//   TEST_ASSERT_EQUAL_STRING("Vector: 10 20 30 \n", buffer);

//   vector_free(v);
// }

/**
 * @brief Test `vector_get` and `vector_get_mut` for valid and invalid indexes.
 */
void test_vector_get_and_get_mut(void)
{
  Vector* v = vector_new();
  vector_push(v, 1);
  vector_push(v, 2);
  vector_push(v, 3);

  const uint16_t* const_elem = vector_get(v, 1);
  TEST_ASSERT_NOT_NULL(const_elem);
  TEST_ASSERT_EQUAL_UINT16(2, *const_elem);

  uint16_t* mut_elem = vector_get_mut(v, 1);
  TEST_ASSERT_NOT_NULL(mut_elem);
  *mut_elem = 99;

  const_elem = vector_get(v, 1);
  TEST_ASSERT_NOT_NULL(const_elem);
  TEST_ASSERT_EQUAL_UINT16(99, *const_elem);

  // Test out-of-bounds access
  const_elem = vector_get(v, 10);
  TEST_ASSERT_NULL(const_elem);

  mut_elem = vector_get_mut(v, 10);
  TEST_ASSERT_NULL(mut_elem);

  vector_free(v);
}

// Main function for running Unity tests
int main(void)
{
  UNITY_BEGIN();  // Initialize Unity framework

  // Run all the test cases
  RUN_TEST(test_vector_new);
  RUN_TEST(test_vector_push);
  RUN_TEST(test_vector_pop);
  RUN_TEST(test_vector_get_and_set);
  RUN_TEST(test_vector_find);
  RUN_TEST(test_vector_reserve);
  RUN_TEST(test_vector_length_and_capacity);
  RUN_TEST(test_vector_push_and_pop);
  RUN_TEST(test_vector_reserve_no_realloc);
  RUN_TEST(test_vector_reserve_exact);
  RUN_TEST(test_vector_with_capacity);
  RUN_TEST(test_vector_free);
  RUN_TEST(test_vector_get_out_of_bounds);
  RUN_TEST(test_vector_get_mut_out_of_bounds);
  RUN_TEST(test_vector_set);
  RUN_TEST(test_vector_set_out_of_bounds);
  RUN_TEST(test_vector_set_valid_index);
  RUN_TEST(test_vector_capacity_after_reallocation);
  RUN_TEST(test_vector_pop_empty);
  RUN_TEST(test_vector_find_different_elements);
  RUN_TEST(test_vector_reserve_large_amount);
  RUN_TEST(test_vector_reserve_smaller_than_current_capacity);
  // RUN_TEST(test_vector_print);
  RUN_TEST(test_vector_get_and_get_mut);

  return UNITY_END();  // Return the test results
}
#endif  // TEST_VECTOR_H