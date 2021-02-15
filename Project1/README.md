Course Mini-Project #1: Matrix-Matrix Multiplication with SIMD Instructions

# Overview

Compile and run using the following command:

```
g++ -g simd_matrix.cpp -Wall -std=c++11 -mavx -o simd_matrix.out && ./simd_matrix.out
```

The code will then ask the user for three inputs. The first input will be size n of the matrix, second input will be whether the data type will be fixed-point integer or a floating point, and the final input will be whether the user wants to use the SIMD based multiplication algorithm or a naive algorithm. 

After the inputs are taken in, the code will generate two matrices of size nxn and fill them with random numbers. After the matrices are intialized and generated, it will then multiply the two matrices using either SIMD or naive algorithm and record the results in a result matrix. Finally, the code will output the time taken for the program to run. 

# Matrix Initialization and Generation

First, the code will initialize matrices with size nxn. A matrix will initially be represented by a double pointer. 

For example a matrix containing short int data types will be declared like this:

```
short int** mat_1 = new short int*[mat_size];
```

Each pointer inside mat_1 will now point to an array of pointers. Meaning, each pointer in mat_1 needs to contain each row of the matrix like this:

```
for(int i = 0; i < mat_size; i++){
    mat_1[i] = new short int[mat_size];
}
```

After the matrix is initialized then a special function rand_matrix() will fill each value inside the matrix with a random integer:

```
void rand_matrix_fix(short int** &input, int size) { //generate random fixed-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = rand() % 100;
    }
  }
}
```

# Naive Matrix Multiplication

Naive method to multiply matrices is to go to each row and column of two matrices then multiply both values and sum them. An algorithm for this operation takes O(n^3) because it utilizes three for loops of size n to calculate the dot products. The naive multiplication for short int matrices is shown below:

```
void fix_matrixmult_normal(short int** &mat1, short int** &mat2, short int** &result, int size) { //traditional matrix mutliplication with fixed-point data
  for (int i = 0; i < size; ++i)
  {
  	for (int j = 0; j < size; ++j)
  	{
  		for (int k = 0; k < size; k++)
  		{
        result[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }
}
```

# SIMD Matrix Multiplication

Using intrinsics for Intel x86, the naive multiplication operation can be sped up drastically while still using a similar algorithm. During matrix multiplication, very simple arithmetic operations are performed repeatedly. These operations are loading, storing, multiplying, and adding data. SIMD allows the code to apply single instruction on large number of data points allowing significant performance gains. For example, instead of performing an operation on each value in a matrix one by one, SIMD can allow n values to be read, operated on, and written back to memory. 

The SIMD 
