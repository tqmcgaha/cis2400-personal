#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// Testing functions. (Assume These Work)
//

// strdup
// merge arrays
void test_str_duplicate();
void test_merge_arrs();

// Given an array (arr) and a length (len), print out the array
// to stdout on a new line.
void print_int_arr(int* arr, int len);

// Given two arrays and their lengths, returns true iff they are equal
bool arrays_eq(int* arr1, int len1, int* arr2, int len2);

///////////////////////////////////////////////////////////////////////////////
// Student Functions.

// given a string src, duplicates it and returns a copy of it
char* str_duplicate(char* src);

// Given two sorted arrays arr1 and arr2, creats a new
// array and returns it to the caller through output
// that contains all elements of both arr1 and arr2 in sorted order
int merge_arrs(int* arr1, int len1, int* arr2, int len2, int** output);

///////////////////////////////////////////////////////////////////////////////
// main
// Runs the test functions
int main(int argc, char** argv) {
  test_str_duplicate();  
  printf("String duplicate tests passed\n");

  test_merge_arrs();  
  printf("Merge tests passed\n");
  
  printf("All tests passed\n");
}

///////////////////////////////////////////////////////////////////////////////
// Student Function Implementations.

char* str_duplicate(char* str) {
  int len = 0;
  while (*str != '\0') {
    len++;
    str++;
  }
  char res[len];
  for (int i = 0; i < len; i++) {
    res[i] = str[i];
  }
  return res;
}

int merge_arrs(int* arr1, int len1, int* arr2, int len2, int** out) {
  int res_len = len1 + len2;
  int res[res_len];

  int index1 = 0;
  int index2 = 0;

  for (int i = 0; i < res_len; i++) {
    if (index1 >= len1) {
      res[i] = arr2[index2];
      index2++;
    } else if (index2 >= len2) {
      res[i] = arr1[index1];
      index1++;
    } else if (arr1[index1] <= arr2[index2]) {
      res[i] = arr1[index1];
      index1++;
    } else {
      res[i] = arr1[index1];
      index1++;
    }
  }

  out = &res;

  return res_len;
}

///////////////////////////////////////////////////////////////////////////////
// Testing Function Implementations.

void test_str_duplicate() {
  char* test1 = "hello there";
  char* test2 = "";
 
  char* res = str_duplicate(test1);
  if (strcmp(res, test1) != 0) {
    printf("Incorrect output. Expected: %s, Actual: %s\n", test1, res);
    exit(EXIT_FAILURE);
  }
  
  res = str_duplicate(test2);
  if (strcmp(res, test2) != 0) {
    printf("Incorrect output. Expected: %s, Actual: %s\n", test2, res);
    exit(EXIT_FAILURE);
  }
}

void test_merge_arrs() {
  int arr1[] = {3, 4, 5};
  int arr2[] = {1, 3, 5, 6, 8};

  int expected[] = {1, 3, 3, 4, 5, 5, 6, 8};

  int* output;
  int res = merge_arrs(arr1, 3, arr2, 5, &output);
  if (!arrays_eq(expected, 8, output, res)) {
    printf("Incorrect output.\n");
    printf("Expected: ");
    print_int_arr(expected, 8);
    printf("Actual: ");
    print_int_arr(output, res);
    exit(EXIT_FAILURE);
  }
}

void print_int_arr(int* arr, int len) {
  printf("[");
  if (len > 0) {
    printf("%d", arr[0]);
    for (int i = 1; i < len; i++) {
      printf(", %d", arr[i]);
    }
  }
  printf("]\n");
}

bool arrays_eq(int* arr1, int len1, int* arr2, int len2) {
  if (len1 != len2) {
    return false;
  }
  for (int i = 0; i < len1; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}
