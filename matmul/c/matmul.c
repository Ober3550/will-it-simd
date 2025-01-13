// A C program to load an arbitrary size matrix from two files multiply them and
// store the result in a third file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Disable printf for performance
#define printf(...) (0)

int main(int argc, char *argv[]) {
// If we generate large enough matrices the performance of reading the file
// header won't matter
#pragma push_macro("printf")
#undef printf
  if (argc != 4) {
    printf("Usage: %s <matrix_a.csv> <matrix_b.csv> <output.csv>\n", argv[0]);
    exit(1);
  }
#pragma pop_macro("printf")
  // Get the filenames for io
  clock_t start_read = clock();
  printf("Filename A: %s\n", argv[1]);
  printf("Filename B: %s\n", argv[2]);
  char *filename_result = argv[3];
  printf("Filename Result: %s\n", filename_result);
  FILE *file_a = fopen(argv[1], "r");
  if (file_a == NULL) {
    printf("Error opening file: %s\n", argv[1]);
    exit(1);
  }
  FILE *file_b = fopen(argv[2], "r");
  if (file_b == NULL) {
    printf("Error opening file: %s\n", argv[2]);
    exit(1);
  }

  // Read the dimensions of the matrices
  int dim_a_x, dim_a_y, dim_b_x, dim_b_y;
  fscanf(file_a, "%d,%d", &dim_a_x, &dim_a_y);
  printf("Dimensions A: %d, %d\n", dim_a_x, dim_a_y);
  fscanf(file_b, "%d,%d", &dim_b_x, &dim_b_y);
  printf("Dimensions B: %d, %d\n", dim_b_x, dim_b_y);

  // Load the matrix data
  printf("Matrix A:\n");
  float *matrix_a = malloc(dim_a_y * dim_a_x * sizeof(float));
  for (int j = 0; j < dim_a_y; j++) {
    for (int i = 0; i < dim_a_x; i++) {
      if (i == 0)
        fscanf(file_a, "%f", &matrix_a[j * dim_a_x + i]);
      else
        fscanf(file_a, ",%f", &matrix_a[j * dim_a_x + i]);
      printf("%0.f ", matrix_a[j * dim_a_x + i]);
    }
    printf("\n");
  }
  printf("Matrix B:\n");
  float *matrix_b = malloc(dim_b_y * dim_b_x * sizeof(float));
  for (int j = 0; j < dim_b_y; j++) {
    for (int i = 0; i < dim_b_x; i++) {
      if (i == 0)
        fscanf(file_b, "%f", &matrix_b[j * dim_b_x + i]);
      else
        fscanf(file_b, ",%f", &matrix_b[j * dim_b_x + i]);
      printf("%0.f ", matrix_b[j * dim_b_x + i]);
    }
    printf("\n");
  }
  fclose(file_a);
  fclose(file_b);
  clock_t end_read = clock();
#pragma push_macro("printf")
#undef printf
  printf("Read time: %.0f ms\n", 1000.0 * (double)(end_read - start_read) / CLOCKS_PER_SEC);
#pragma pop_macro("printf")

  clock_t start_mul = clock();
  // Check if the matrices are valid
  if (dim_a_x != dim_b_y) {
    printf("Cannot multiply matrices\n");
    exit(1);
  }

  // Multiply the matrices
  long int total_operations = 0;
  float *result = malloc(dim_a_y * dim_b_x * sizeof(float));
  for (int j = 0; j < dim_a_y; j++) {
    for (int i = 0; i < dim_b_x; i++) {
      result[j * dim_a_y + i] = 0;
      for (int k = 0; k < dim_a_x; k++) {
        const float a = matrix_a[j * dim_a_x + k];
        const float b = matrix_b[k * dim_b_x + i];
        result[j * dim_b_x + i] += a * b;
        printf("result[%d][%d] %0.f += [%d][%d] %0.f * [%d][%d] %0.f\n", j, i,
               result[j * dim_a_y + i], j, k, a, k, i, b);
        total_operations++;
      }
      printf("\n");
    }
  }
  clock_t end_mul = clock();
#undef printf
  printf("Matmul time: %.0f ms\n",
         1000.0 * (double)(end_mul - start_mul) / CLOCKS_PER_SEC);
  printf("Result dimensions: %d %d\n", dim_a_y, dim_b_x);
  printf("Total operations: %ld\n", total_operations);

  clock_t start_write = clock();
  // Store the result
  FILE *file_result = fopen(filename_result, "w");
  if (file_result == NULL) {
    printf("Error opening file: %s\n", filename_result);
    exit(1);
  }
  fprintf(file_result, "%d,%d\n", dim_b_x, dim_a_y);
  for (int j = 0; j < dim_a_y; j++) {
    for (int i = 0; i < dim_b_x; i++) {
      if (i == 0)
        fprintf(file_result, "%0.f", result[j * dim_b_x + i]);
      else
        fprintf(file_result, ",%0.f", result[j * dim_b_x + i]);
    }
    fprintf(file_result, "\n");
  }
  fclose(file_result);
  clock_t end_write = clock();
  printf("Write time: %.0f ms\n",
         1000.0 * (double)(end_write - start_write) / CLOCKS_PER_SEC);
  return 0;
}