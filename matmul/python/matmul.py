import sys
import numpy
import time


def current():
    return time.time_ns() // 1000000


if __name__ == '__main__':
    start_read = current()
    filename_a = sys.argv[1]
    filename_b = sys.argv[2]
    filename_result = sys.argv[3]
    matrix_a, matrix_b = None, None
    with open(filename_a) as f:
        matrix_a = numpy.loadtxt(f, delimiter=',', skiprows=1)
    with open(filename_b) as f:
        matrix_b = numpy.loadtxt(f, delimiter=',', skiprows=1)
    end_read = current()
    print(f'Read time: {end_read - start_read} ms')
    start_matmul = current()
    result = numpy.matmul(matrix_a, matrix_b)
    end_matmul = current()
    print(f'Matmul time: {end_matmul - start_matmul} ms')
    start_write = current()
    with open(filename_result, 'w') as f:
        f.write(f'{result.shape[0]},{result.shape[1]}\n')
        numpy.savetxt(f, result, delimiter=',', fmt='%0.f')
    end_write = current()
    print(f'Write time: {end_write - start_write} ms')
