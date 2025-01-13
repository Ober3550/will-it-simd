import numpy as np
import csv

size = 1024

# Generate a 256x256 matrix with random floating point numbers
matrix = np.random.rand(size, size)

# Define the file path
file_path = f'./matrix_b_{size}.csv'

# Write the matrix to a CSV file
with open(file_path, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow([size, size])
    for row in matrix:
        writer.writerow(row)