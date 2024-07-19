#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef VECTOR_H
#  define VECTOR_H

/**
 * @file vector.h
 * @brief A header file for a generic dynamic array implementation.
 *
 * This header file defines a dynamic array (Vector) data structure for managing
 * a collection of elements of a specified type. It includes the type
 * definition, and function prototypes for creating, manipulating, and
 * destroying a Vector. The implementation supports basic operations like push,
 * pop, access, and resizing.
 */

/**
 * @typedef Vector
 * @brief A structure representing a dynamic array.
 *
 * This structure holds the internal state of a Vector, including:
 * - `data`: A pointer to the array of elements.
 * - `capacity`: The total number of elements that the Vector can hold before
 * resizing is required.
 * - `length`: The current number of elements in the Vector.
 *
 * @tparam VECTOR_H_i_TYPE The type of the elements stored in the Vector. For
 * example, `uint16_t`, `int`, etc.
 */
typedef struct Vector {
  VECTOR_H_i_TYPE*
      data; /**< A pointer to an array of elements of type VECTOR_H_i_TYPE */
  size_t capacity; /**< The total capacity of the Vector, i.e., the number of
                   elements it can hold */
  size_t length; /**< The current number of elements in the Vector */
} Vector;

#  define VECTOR_INIT_CAPACITY \
    4 /**< Initial capacity for the Vector upon creation */

/**
 * @brief Frees the memory allocated for a Vector.
 *
 * This function deallocates the memory used by the Vector's internal array as
 * well as the Vector itself. It is crucial to call this function to prevent
 * memory leaks when the Vector is no longer needed.
 *
 * @param self A pointer to the Vector instance to be freed.
 */
void vector_free(Vector* /*self*/);

/**
 * @brief Error message for failed dynamic memory allocation.
 *
 * This macro defines the error message displayed when dynamic memory allocation
 * fails.
 */
#  define FAILED_ALLOCATION_ERR_MSG \
    ("error: failed allocation of dynamic memory")

/**
 * @brief Macro to get the name of the print function for the element type.
 *
 * This macro helps in expanding to the specific print function for the Vector's
 * element type. If `TYPE` expands to `uint16_t`, then `PRINT_ANY(TYPE)` becomes
 * `print_uint16_t`.
 */
#  define PRINT_ANY(TYPE) PRINT_FUNCTION_NAME(TYPE)

/**
 * @brief Macro to define the name of the print function based on the type.
 *
 * If `TYPE` expands to `uint16_t`, `PRINT_FUNCTION_NAME(TYPE)` becomes
 * `print_uint16_t`.
 */
#  define PRINT_FUNCTION_NAME(TYPE) print_##TYPE
#  define PRINT_FUNCTION_DECLARATION(TYPE) \
    extern void PRINT_ANY(VECTOR_H_i_TYPE)(VECTOR_H_i_TYPE);

/**
 * Error that plagued this repository for very long
 * /home/runner/.../test/source/test_vector.c:13:6: error: conflicting types for
 * 'print_uint16_t' void print_uint16_t(uint16_t elem)
 *     ^
 * /home/runner/.../include/vector/vector.h:158:5: note: previous implicit
 * declaration is here CALL_PRINT_FUNCTION(VECTOR_H_i_TYPE)
 *     ^
 * /home/runner/.../include/vector/vector.h:82:39: note: expanded from macro
 * 'CALL_PRINT_FUNCTION' #  define CALL_PRINT_FUNCTION(i_type)
 * PRINT_FUNCTION_NAME(i_type)
 *                                       ^
 * /home/runner/.../include/vector/vector.h:78:37: note: expanded from macro
 * 'PRINT_FUNCTION_NAME' #  define PRINT_FUNCTION_NAME(TYPE) print_##TYPE
 *                                     ^
 * <scratch space>:33:1: note: expanded from here
 * print_uint16_t
 *
 * Seems like the trick is to write a macro that expands to the function
 * declaration: `PRINT_FUNCTION_DECLARATION(VECTOR_H_i_TYPE)` -> `extern void
 * print_uint16_t(uint16_t);`
 *
 * And also, note that the function declaration should be `extern void
 * print_uint16_t(uint16_t);` NOT `extern void print_uint16_t(uint16_t*);`
 *
 */
PRINT_FUNCTION_DECLARATION(VECTOR_H_i_TYPE)

#  define CALL_PRINT_FUNCTION(i_type) PRINT_FUNCTION_NAME(i_type)
/**
 * @brief Creates a new Vector instance with the default initial capacity.
 *
 * This function allocates memory for a new Vector and initializes it with the
 * default capacity defined by `VECTOR_INIT_CAPACITY`. It also allocates memory
 * for storing elements. The `data` pointer is allocated with space for the
 * initial number of elements.
 *
 * @return A pointer to the newly created Vector instance.
 */
Vector* vector_new(void)
{
  static_assert(sizeof(Vector) != 0, "`sizeof(Vector)` == 0`");
  static_assert(sizeof(VECTOR_H_i_TYPE) != 0,
                "`sizeof(VECTOR_H_i_TYPE)` == 0`");
  Vector* result = (Vector*)malloc(sizeof(Vector));
  if (result == NULL) {
    printf(FAILED_ALLOCATION_ERR_MSG);
    exit(1);
  };
  result->length = 0;
  result->capacity = VECTOR_INIT_CAPACITY;

  result->data =
      (VECTOR_H_i_TYPE*)malloc(VECTOR_INIT_CAPACITY * sizeof(VECTOR_H_i_TYPE));
  if (result->data == NULL) {
    printf(FAILED_ALLOCATION_ERR_MSG);
    free(result);
    exit(1);
  };
  return result;
}

/**
 * @brief Creates a Vector with a specified initial capacity.
 *
 * This function initializes a Vector with a given initial capacity, but does
 * not allocate memory for elements. The `data` pointer is set to `NULL`,
 * `capacity` is set to the provided value, and `length` is initialized to 0.
 *
 * @param capacity The initial capacity of the Vector.
 * @return A Vector instance with the specified initial capacity.
 */
Vector* vector_with_capacity(size_t capacity)
{
  Vector* result;
  static_assert(sizeof(Vector) != 0, "`sizeof(Vector)` == 0`");
  result = vector_new();
  result->capacity = capacity;
  return result;
}

/**
 * @brief Prints the elements of the Vector.
 *
 * This function prints each element of the Vector from the first to the last
 * element. It uses the `PRINT_ANY` macro to call the appropriate print function
 * based on the `VECTOR_H_i_TYPE`.
 *
 * @param self A pointer to the Vector instance to be printed.
 */
void vector_print(Vector const* self)
{
  if (self == NULL || self->data == NULL) {
    printf(
        "error: attempt to print from a null vector or uninitialized data "
        "array.\n");
    return;
  }
  size_t i, length;
  VECTOR_H_i_TYPE elem;
  length = self->length;
  printf("Vector: ");
  for (i = 0; i < length; i++) {
    elem = self->data[i];
    CALL_PRINT_FUNCTION(VECTOR_H_i_TYPE)
    (elem); /**< Call the specific print function
         for VECTOR_H_i_TYPE */
    printf(" ");
  }
  printf("\n");
}

/**
 * @brief Adds a new element to the end of the Vector.
 *
 * This function appends a new element to the end of the Vector. If the Vector's
 * capacity is exceeded, it automatically resizes the Vector to accommodate the
 * new element.
 *
 * @param self A pointer to the Vector instance.
 * @param element The element to be added to the Vector.
 */
void vector_push(Vector* self, VECTOR_H_i_TYPE element)
{
  static_assert(sizeof(VECTOR_H_i_TYPE) != 0,
                "`sizeof(VECTOR_H_i_TYPE)` == 0`");
  if (self->length >= self->capacity) {
    self->capacity = self->capacity * 2;
    VECTOR_H_i_TYPE* data;

    data = (VECTOR_H_i_TYPE*)realloc(self->data,
                                     self->capacity * sizeof(VECTOR_H_i_TYPE));
    if (data == NULL) {
      printf(FAILED_ALLOCATION_ERR_MSG);
      goto failure;
    };
    self->data = data;
  }
  self->data[self->length] = element;
  self->length++;
  return;

failure:
  vector_free(self);
  exit(1);
}

/**
 * @brief Removes and returns the last element of the Vector.
 *
 * This function decreases the `length` of the Vector by one and returns the
 * last element. If the Vector is empty, it returns `NULL`.
 *
 * @param self A pointer to the Vector instance.
 * @return The last element of the Vector.
 */
VECTOR_H_i_TYPE* vector_pop(Vector* self)
{
  if (self->length == 0) {
    return NULL;
  }

  self->length--;
  return &self->data[self->length];
}

/**
 * @brief Retrieves a constant pointer to the element at the specified index.
 *
 * This function returns a pointer to the element at the given index. The
 * pointer is `const` to prevent modification of the element's value.
 *
 * @param self A pointer to the Vector instance.
 * @param index The index of the element to be retrieved.
 * @return A constant pointer to the element at the specified index, or `NULL`
 * if the index is out of bounds.
 */
const VECTOR_H_i_TYPE* vector_get(Vector const* self, size_t index)
{
  if (index < self->length) {
    return &self->data[index];
  }
  return NULL;
}

/**
 * @brief Retrieves a mutable pointer to the element at the specified index.
 *
 * This function returns a pointer to the element at the given index. The
 * pointer is mutable to allow modification of the element's value.
 *
 * @param self A pointer to the Vector instance.
 * @param index The index of the element to be retrieved.
 * @return A mutable pointer to the element at the specified index, or `NULL` if
 * the index is out of bounds.
 */
VECTOR_H_i_TYPE* vector_get_mut(Vector* self, size_t index)
{
  if (index < self->length) {  //  error: comparison of unsigned expression in
                               //  ‘>= 0’ is always true [-Werror=type-limits]
    return &self->data[index];
  }
  return NULL;
}

/**
 * @brief Sets the value of the element at the specified index.
 *
 * This function assigns a new value to the element at the given index.
 * The index must be within the current bounds of the Vector.
 *
 * @param self A pointer to the Vector instance.
 * @param index The index of the element to be set.
 * @param element The new value to be assigned to the element.
 */
void vector_set(Vector* self, size_t index, VECTOR_H_i_TYPE element)
{
  if (index < self->length) {
    self->data[index] = element;
  }
}

/**
 * @brief Frees the memory allocated for the Vector.
 *
 * This function deallocates the memory used by the Vector's internal array and
 * the Vector itself. It should be called when the Vector is no longer needed to
 * prevent memory leaks.
 *
 * @param self A pointer to the Vector instance to be freed.
 */
void vector_free(Vector* self)
{
  if (self->data != NULL) {
    free(self->data);
    self->data = NULL;
  }
  if (self != NULL) {
    free(self);
    self = NULL;
  }
}

/**
 * @brief Retrieves the current number of elements in the Vector.
 *
 * This function returns the number of elements that are currently stored in the
 * Vector. The `length` member of the Vector structure reflects this count.
 *
 * @param self A pointer to the Vector instance.
 * @return The number of elements in the Vector.
 */
size_t vector_length(Vector* self)
{
  return self->length;
}

/**
 * @brief Retrieves the current capacity of the Vector.
 *
 * This function returns the maximum number of elements that the Vector can hold
 * without needing to resize. The `capacity` member of the Vector structure
 * reflects this value.
 *
 * @param self A pointer to the Vector instance.
 * @return The capacity of the Vector.
 */
size_t vector_capacity(Vector* self)
{
  return self->capacity;
}

/**
 * @brief Reserves additional space in the Vector for more elements.
 *
 * This function increases the capacity of the Vector if the current capacity is
 * insufficient to accommodate the specified reserve amount. The Vector will be
 * resized to ensure that there is enough space for additional elements.
 *
 * @param self A pointer to the Vector instance.
 * @param reserve The additional space to reserve in the Vector.
 * @return 1 if the reserve was successful, 0 if no additional space was needed.
 */
int vector_reserve(Vector* self, size_t reserve)
{
  VECTOR_H_i_TYPE* data;
  static_assert(sizeof(VECTOR_H_i_TYPE) != 0, "`sizeof(VECTOR_H_i_TYPE)` == 0");
  assert(reserve != 0);
  if ((self->capacity - self->length) >= reserve) {
    return 0;
  }
  while (!((self->capacity - self->length) > reserve)) {
    self->capacity *= 2;
  }
  data = (VECTOR_H_i_TYPE*)realloc(self->data,
                                   self->capacity * sizeof(VECTOR_H_i_TYPE));
  if (data == NULL) {
    printf(FAILED_ALLOCATION_ERR_MSG);
    exit(1);
  }
  self->data = data;
  return 1;
}

/**
 * @brief Finds an element in the Vector.
 *
 * This function searches for the specified element in the Vector and updates
 * the result flag. If the element is found, it returns a pointer to the element
 * and sets `result` to 1. If not, it returns `NULL` and sets `result` to 0.
 *
 * @param self A pointer to the Vector instance.
 * @param element The element to search for in the Vector.
 * @param result A pointer to an integer that will be set to 1 if the element is
 * found, or 0 otherwise.
 * @return A pointer to the found element, or `NULL` if the element is not
 * present.
 */
VECTOR_H_i_TYPE* vector_find(Vector* self,
                             const VECTOR_H_i_TYPE element,
                             int* result)
{
  *result = 0;
  size_t i;
  for (i = 0; i < self->length; i++) {
    if ((self->data[i]) == element) {
      *result = 1;
      return &self->data[i];
    }
  }
  return NULL;
}

#endif  // VECTOR_H
