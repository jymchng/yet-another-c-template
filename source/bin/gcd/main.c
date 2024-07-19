#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "gcd.h"

#define BUFFER_SIZE 100

// Returns the number of bytes read
extern inline ptrdiff_t read_one_integer(char* input, char* end, int* result)
{
  int x = 0;
  char* start = input;
  while (start != end && (*start < '0' || *start > '9'))
    ++start;
  while (start != end && (*start >= '0' && *start <= '9')) {
    x = (x << 3) + (x << 1) + *(start++) - '0';
  }
  *result = x;
  return start - input;
}

int main(void)
{
  int num1, num2;
  char input[BUFFER_SIZE];
  printf("Enter two integers: ");

#ifdef _MSC_VER
  if (scanf_s("", &input, BUFFER_SIZE)) {
    fprintf(stderr, "Invalid input. Please enter two integers.\n");
    return EXIT_FAILURE;
  }
  if (fgets(input, sizeof(input), stdin) == NULL) {
    fprintf(stderr, "Error reading input.\n");
    return EXIT_FAILURE;
  }

  char* ptr = input;
  while (*ptr) {
    if (!isdigit(*ptr) && !isspace(*ptr) && *ptr != '-') {
      fprintf(stderr,
              "Invalid input. Please enter two integers.\nYou have "
              "entered %s",
              input);
      return EXIT_FAILURE;
    }
    ptr++;
  }

  char* start = input;
  char* end = start + BUFFER_SIZE;

  start += read_one_integer(start, end, &num1);
  start += read_one_integer(start, end, &num2);

#else  // GCC OR CLANG
  // Read the entire line of input
  if (fgets(input, sizeof(input), stdin) == NULL) {
    fprintf(stderr, "Error reading input.\n");
    return EXIT_FAILURE;
  }

  // Check if the input contains exactly two integers
  char* ptr = input;
  while (*ptr) {
    if (!isdigit(*ptr) && !isspace(*ptr) && *ptr != '-') {
      fprintf(stderr,
              "Invalid input. Please enter two integers.\nYou have entered %s",
              input);
      return EXIT_FAILURE;
    }
    ptr++;
  }

  if (sscanf(input, "%d %d", &num1, &num2) != 2) {
    fprintf(stderr, "Invalid input. Please enter two integers.\n");
    return EXIT_FAILURE;
  }
#endif  // _MSC_VER

  printf("GCD of %d and %d is %d\n", num1, num2, gcd(num1, num2));

  return EXIT_SUCCESS;
}