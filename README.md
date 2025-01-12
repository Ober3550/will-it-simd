# Will it simd?

The goal of this project is to evaluate the automatic and manual simd features available in various languages. This is a response to Casey Muratori's and Primagens reaction to poor and useless micro benchmarks that don't truly evaluate anything useful about modern languages that use common backend optimizers. We will be evaluating whether a lanugage can automatically simd code aswell as whether there are tools to allow the dev to write inline assembly or intrinsics to use simd. 

## What algorithm are we testing?

If we're writing optimized algorithms in each language we may aswell make the implementation we're producing useful. Some useful algorithms we could try is a hashing algorithm, matrix multiply or a flat hashmap. 

How to progress the project
1. Choose the algorithm
2. Write a naive program (likely in c) to run the algorithm
3. Decide on the common io process (likely byte file read to try avoiding testing json parsers)
4. Setup a test suite to validate the output
5. Optimize the language as much as possible