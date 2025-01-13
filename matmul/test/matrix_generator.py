import numpy as np
import csv
import sys

name = sys.argv[1]
size = int(sys.argv[2])

# Generate a nxn matrix with random floating point numbers
matrix = np.random.rand(size, size)

# Define the file path
file_path = f'./{name}_{size}.csv'

# Write the matrix to a CSV file
with open(file_path, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow([size, size])
    for row in matrix:
        writer.writerow(row)