// A C program to load an arbitrary size matrix from two files multiply them and
// store the result in a third file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_multiply(float *matrix_a, int dim_a_x, int dim_a_y,
                     float *matrix_b_transpose, int dim_b_x, int dim_b_y,
                     float *result) {
  long int total_operations = 0;
  printf("Result dimensions: %d %d\n", dim_a_y, dim_b_x);
  for (int j = 0; j < dim_a_y; j++) {
    for (int i = 0; i < dim_b_x; i++) {
      result[j * dim_a_y + i] = 0;
      for (int k = 0; k < dim_a_x; k++) {
        const float a = matrix_a[j * dim_a_x + k];
        const float b = matrix_b_transpose[i * dim_b_y + k];
        result[j * dim_a_y + i] += a * b;
        // printf("result[%d][%d] %0.f += [%d][%d] %0.f * [%d][%d] %0.f\n", j,
        // i,
        //        result[j * dim_a_y + i], j, k, a, i, k, b);
        total_operations++;
      }
      // printf("\n");
    }
  }
  printf("Total operations: %ld\n", total_operations);
}

// Transpose of non-square matrices is non-trivial
void matrix_transpose(float *matrix, int dim_x, int dim_y) {
  for (int i = 1; i < dim_x; i++) {
    /* stop the inner loop when b == a */
    for (int j = 0; j < i; j++) {
      int tmp = matrix[j * dim_y + i];
      matrix[j * dim_y + i] = matrix[i * dim_y + j];
      matrix[i * dim_y + j] = tmp;
    }
  }
}

float *read_matrix(char *filename, int *dim_x, int *dim_y) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file: %s\n", filename);
    exit(1);
  }
  fscanf(file, "%d,%d", dim_x, dim_y);
  float *matrix = malloc(*dim_y * *dim_x * sizeof(float));
  for (int j = 0; j < *dim_y; j++) {
    for (int i = 0; i < *dim_x; i++) {
      if (i == 0)
        fscanf(file, "%f", &matrix[j * *dim_x + i]);
      else
        fscanf(file, ",%f", &matrix[j * *dim_x + i]);
    }
  }
  fclose(file);
  return matrix;
}

void write_matrix(char *filename, float *matrix, int dim_x, int dim_y) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file: %s\n", filename);
    exit(1);
  }
  fprintf(file, "%d,%d\n", dim_x, dim_y);
  for (int j = 0; j < dim_y; j++) {
    for (int i = 0; i < dim_x; i++) {
      if (i == 0)
        fprintf(file, "%0.f", matrix[j * dim_x + i]);
      else
        fprintf(file, ",%0.f", matrix[j * dim_x + i]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

int main(int argc, char *argv[]) {
  // If we generate large enough matrices the performance of reading the file
  // header won't matter
  if (argc != 4) {
    printf("Usage: %s <matrix_a.csv> <matrix_b.csv> <output.csv>\n", argv[0]);
    exit(1);
  }

  // Read the matrices
  clock_t start_read = clock();
  int dim_a_x, dim_a_y, dim_b_x, dim_b_y;
  float *matrix_a = read_matrix(argv[1], &dim_a_x, &dim_a_y);
  float *matrix_b = read_matrix(argv[2], &dim_b_x, &dim_b_y);

  // Check if the matrices are valid
  if (dim_a_x != dim_b_y) {
    printf("Cannot multiply matrices\n");
    exit(1);
  }
  clock_t end_read = clock();
  printf("Read time: %.0f ms\n",
         1000.0 * (double)(end_read - start_read) / CLOCKS_PER_SEC);

  // Transpose the second matrix for performance
  clock_t start_transpose = clock();
  matrix_transpose(matrix_b, dim_b_x, dim_b_y);
  clock_t end_transpose = clock();
  printf("Transpose time: %.0f ms\n",
         1000.0 * (double)(end_transpose - start_transpose) / CLOCKS_PER_SEC);

  // Multiply the matrices
  float *result = malloc(dim_a_y * dim_b_x * sizeof(float));
  clock_t start_mul = clock();
  matrix_multiply(matrix_a, dim_a_x, dim_a_y, matrix_b, dim_b_x, dim_b_y,
                  result);
  clock_t end_mul = clock();

  // Write the result
  printf("Matmul time: %.0f ms\n",
         1000.0 * (double)(end_mul - start_mul) / CLOCKS_PER_SEC);
  clock_t start_write = clock();
  write_matrix(argv[3], result, dim_a_y, dim_b_x);
  clock_t end_write = clock();
  printf("Write time: %.0f ms\n",
         1000.0 * (double)(end_write - start_write) / CLOCKS_PER_SEC);
  return 0;
}