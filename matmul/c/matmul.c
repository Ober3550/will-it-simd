// Write a C program to load an arbitrary size matrix from two files and
// multiply them together The output will be written to a third file
#include <stdio.h>
#include <stdlib.h>

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
      printf("%.2f ", matrix_a[j * dim_a_x + i]);
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
      printf("%.2f ", matrix_b[j * dim_b_x + i]);
    }
    printf("\n");
  }
  fclose(file_a);
  fclose(file_b);

  // Check if the matrices are valid
  if (dim_a_x != dim_b_y) {
    printf("Cannot multiply matrices\n");
    exit(1);
  }

  // Multiply the matrices
  int total_operations = 0;
  float *result = malloc(dim_a_y * dim_b_x * sizeof(float));
  for (int j = 0; j < dim_a_y; j++) {
    for (int i = 0; i < dim_b_x; i++) {
      result[j * dim_a_y + i] = 0;
      for (int k = 0; k < dim_a_x; k++) {
        printf("\nresult[%d][%d] += [%d][%d] %f * [%d][%d] %f\n", j, i, j, k, matrix_a[j * dim_a_x + k], k, i, matrix_b[k * dim_b_x + i]);
        result[j * dim_a_x + i] += matrix_a[j * dim_a_x + k] * matrix_b[k * dim_b_x + i];
        total_operations++;
      }
      printf("%.2f ", result[j * dim_a_x + i]);
    }
    printf("\n");
  }
  #undef printf
  printf("Dimensions Result: %d %d\n", dim_a_y, dim_b_x);
  printf("Total operations: %d\n", total_operations);

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
        fprintf(file_result, "%.2f", result[j * dim_a_x + i]);
      else
        fprintf(file_result, ",%.2f", result[j * dim_a_x + i]);
    }
    fprintf(file_result, "\n");
  }
  return 0;
}