Course Mini-Project #1: Matrix-Matrix Multiplication with SIMD Instructions

##Overview

Compile and run using the following command:

```
g++ -g simd_matrix.cpp -Wall -std=c++11 -mavx -o simd_matrix.out && ./simd_matrix.out
```

The code will then ask the user for three inputs. The first input will be size n of the matrix, second input will be whether the data type will be fixed-point integer or a floating point, and the final input will be whether the user wants to use the SIMD based multiplication algorithm or a naive algorithm. 

After the inputs are taken in, the code will generate a two matrices of size nxn and fill them with random numbers. After the matrices are intialized and generated, it will then multiply the two matrices using either SIMD or naive algorithm and record the results in a result matrix. Finally, the code will output the time taken for the program to run. 

