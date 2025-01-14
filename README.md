# Will it simd?

The goal of this project is to evaluate the automatic and manual simd features available in various languages. This is a response to Casey Muratori's and Primagens reaction to poor and useless micro benchmarks that don't truly evaluate anything useful about modern languages that use common backend optimizers. We will be evaluating whether a lanugage can automatically simd code aswell as whether there are tools to allow the dev to write inline assembly or intrinsics to use simd.

## Results

Below is a table of the results from checking https://godbolt.org/
Timing information is on the order O(n^3) with n=2000.

| lang compiler target feature                |          will it simd? | read time | run time  | write time |
|---------------------------------------------|------------------------|-----------|-----------|------------|
| c gcc arm                                   |     :white_check_mark: |    1.030s |   10.082s |     0.481s |
| c gcc arm transpose                         |     :white_check_mark: |    1.030s |    7.317s |     0.481s |
| c clang arm                                 |     :white_check_mark: |    1.033s |    9.772s |     0.461s |
| c clang arm transpose                       |     :white_check_mark: |    1.027s |    7.162s |     0.462s |
| zig                                         |     :white_check_mark: |    0.373s |    7.639s |     0.157s |
| zig transpose                               |     :white_check_mark: |    0.365s |    7.161s |     0.162s |
| javascript node arm                         |                    :x: |    2.649s | 1m17.776s |     3.057s |
| javascript node arm float32 array           | :large_orange_diamond: |    2.649s |   41.204s |     3.057s |
| javascript node arm float32 array transpose |     :white_check_mark: |    2.858s |    7.866s |     3.316s |

## What algorithm are we testing?

If we're writing optimized algorithms in each language we may aswell make the implementation we're producing useful. Some useful algorithms we could try is a hashing algorithm, matrix multiply or a flat hashmap.

How to progress the project

1. Choose the algorithm - For simplicity we're just doing matrix multiply to start
2. Write a naive program to run the algorithm - Wrote a small one in C
3. Make sure you're heap allocating your matrix otherwise the stack won't fit the larger ones
4. Decide on the common io process - Using a csv with a 2 int header specifying the n \* m matrix size the data is in a float32 string format
5. Output relevant input arguments, indices, element values as debugging but turn off for performance.
6. Print result matrix size and total operations at the end - Yes this slightly adds to the performance cost
7. Setup a test suite to validate the output - `validate_tests.sh` will build and run the app and will check against an expected_result.csv with the linux `cmp` command. In future this may need to be changed as it's doing string comparison not float comparison
8. Setup a performance suite to check the performance of the app - `performance_tests.sh` will build the app and then generate two 1500x1500 matrices which will then be passed as filenames to your app. The result file is not validated. But you should output how many operations your hot path completed to check it's correct

# Notes

- Matrix multiply works by Matrix A being column-major and Matrix B being row-major. Simply changing operations on Matrix B to also be column-major by loading it in as it's transpose. We can see a performance improvement from `5.469s` of runtime to `4.081s`
- Since we're using test data we can pretend that Matrix B is a transpose instead of paying the cost of rearranging the data when loading from file
- All bash files execute relative to themselves with the `cd "$(dirname "$0")"` command
- Test files are generated instead of stored in the repo
- The matrix generator uses numpy but could use something else
- Try and make the bash files install relevant dependencies to run each test
- Try and include logging for debugging and validation but have a way to disable for running algorithm so we're not testing streaming data to terminal
- It seems like there's a bug with bun write streams because it's performance is very poor. However, I noticed that the performance of bun was the same if you used a regular javascript array or a specific float32 array. Perhaps bun is auto casting to native when it sees arrays full of the same data type?
