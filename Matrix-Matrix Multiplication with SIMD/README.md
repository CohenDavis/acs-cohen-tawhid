Course Mini-Project #1: Matrix-Matrix Multiplication with SIMD Instructions

# Overview

Compile and run using the following command:

```
g++ -g simd_matrix.cpp -Wall -std=c++11 -mavx2 -o simd_matrix.out && ./simd_matrix.out
```

The code will then ask the user for three inputs. The first input will be size n of the matrix (the matrices are square, and thus only one dimensional input is required), the second input will be whether the data type will be fixed-point integer or a floating-point, and the final input will be whether the user wants to use the SIMD based multiplication algorithm or a naive algorithm.

After the inputs are taken in, the code will generate two matrices of size nxn and fill them with random numbers. After the matrices are initialized and generated, it will then multiply the two matrices using either the SIMD or naive algorithm and record the results in a result matrix. Finally, the code will output the time taken for the program to run.

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

After the matrix is initialized, then a special function rand_matrix() will fill each value inside the matrix with a random integer:

```
void rand_matrix_fix(short int** &input, int size) { //generate random fixed-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = rand() % 100;
    }
  }
}
```
For floating-point data, this integer is converted to a float.

# Naive Matrix Multiplication

The naive method to multiply matrices is to go to each row and column of the two matrices, then multiply both values and sum them. An algorithm for this operation takes O(n^3) because it utilizes three "for" loops of size n to calculate the dot products. The naive multiplication for short int (2-byte fixed-point) matrices is shown below:

```
void fix_matrixmult_normal(short int** &mat1, short int** &mat2, short int** &result, int size) { //traditional matrix multiplication with fixed-point data
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

Using intrinsics for Intel x86, the naive multiplication operation can be sped up drastically while still using a similar algorithm. During matrix multiplication, very simple arithmetic operations are performed repeatedly. These operations are loading, storing, multiplying, and adding data. SIMD allows the code to apply a single instruction on a large number of data points, allowing significant performance gains. For example, instead of performing an operation on each value in a matrix one by one, SIMD can allow multiple values to be read, operated on, and written back to memory. The number of values that can be operated on at one time depends on the type of data being used. In this code, both AVX and AVX2 intrinsics are utilized.

The SIMD algorithm for multiplying floating-point matrices is very similar to the naive implementation. The key difference is it will load, multiply, and store multiple data points into a 256 bit register. Due to the types of data being processed, there are two different SIMD-infused implementations - one for each type of data; while these implementations leverage slightly different intrinsics, they are functionally equivalent, and will thus be addressed as a singular SIMD implementation in much of the rest of this discussion.

# Performance Comparison

### Hardware Used During Testing

Dell XPS-9560:
* i7-7700HQ (4C/8T, 2.80GHz Base/3.80GHz Boost)
* GTX 1050 (2GB GDDR5)
* 16GB RAM
* 512 GB NVMe SSD
* Windows 10 64-bit

### Testing Results

Tests were performed with various nxn matrices containing either floating-point or fixed-point data to compare the time it took to multiply them. The following table shows the time in seconds it took to multiply different matrices using the SIMD or naive multiplication method.

Matrix width n | Naive-Float (time in seconds) | SIMD-Float (time in seconds) | Percent speed-up (%)
------------ | ------------- | ------------- | -------------
1000 | 7.6 | 1.1 | 85.5
2000 | 206.8 | 20.0 | 90.3
3000 | 349.8 | 43.6 | 87.5
4000 | 906.3 | 111.6 | 87.7
5000 | 1886.1 | 236.2 | 87.5
6000 | Not tested | 463.9| N/A
7000 | Not tested | 674.2 | N/A
8000 | Not tested | 1030.5 | N/A
9000 | Not tested | 1502.2 | N/A
10000 | > 43,200 | 1993.5 | > 95.4

Matrix width n | Naive-Int (time in seconds) | SIMD-Int (time in seconds) | Percent speed-up (%)
------------ | ------------- | ------------- | -------------
1000 | 6.9 | 1.5 | 78.3
2000 | 88.4 | 18.5 | 79.1
3000 | 312.6 | 66.1 | 78.9
4000 | 822.2 | 163.8 | 80.1
5000 | 1747.4 | 360.3 | 79.4
6000 | Not tested | 638.4 | N/A
7000 | Not tested | 1020.7 | N/A
8000 | Not tested | 1592.9 | N/A
9000 | Not tested | 2197.2 | N/A
10000 | > 43,200 | 3957.6 | > 90.8

# Analysis & Conclusions

In measuring the performance differential between the SIMD and naive matrix multiplication algorithms/implementations, experimentation was completed with square matrices of widths/lengths measuring between 1,000 and 10,000 elements (in increments of 1,000). For both the fixed-point and floating-point data types, all 1,000 to 10,000 matrix sizes were tested using the SIMD implementations. With the naive implementation however, testing was only done for sizes 1,000-5,000, and then 10,000. This is due to the exceptionally long amount of time it would have taken for the naive implementation to complete the multiplication process. The 10,000 matrix size was tested for both data types using the naive implementation to demonstrate how poorly this method scaled up, and the processes had not finished after 12 hours in each case; experimentation was thus cut off at that point. As mentioned previously, random matrices of the proper size and data type were generated for this experimentation, and due to the amount of data being manipulated, the result matrix was simply kept in the heap and not output in a user-viewable manner. The key user-desired metric here, after all, is the runtime/s of the different algorithms for various matrix configurations - not data related to the matrix contents.

As evident in the tables above, the SIMD-infused implementation shows significant gains in speed over its naive counterpart. For the single-precision floating-point data, the time taken to complete the multiplication was often almost (and sometimes better than) 90% lower. For reference, percent speed-up was calculated as follows: % SPEED-UP = (NEW - OLD)/OLD x 100. For the fixed-point (short integer) data, the execution time reduction wasn't quite as high as with floating-point, but still hovered around the 80% mark for most of the matrix sizes tested. For both data types however, performance gains were much more substantial for matrix sizes near and exceeding 10,000. As mentioned, after allowing the naive implementation to run with that larger 10,000 size for 12 hours, the process was still not complete and was thus terminated. Therefore, while it is impossible to know exactly how much of a speed-up there is for this matrix size, it is known to be greater than 95% for floating-point data and greater than 90% for fixed-point data.

As can also be seen in the collected data, while execution took less time for fixed-point data than floating-point data when utilizing the naive algorithm, this was reversed for the SIMD implementations where the floating-point took less time. It is unknown precisely why this occurred, but due to how similar the floating and fixed implementations are, the difference likely came down to specific choices in intrinsic functions. With more optimal intrinsic choices and further testing, it is reasonable to assume that the execution times for fixed-point data could be further reduced. Were further development to be completed on the higher efficiency algorithm/s, there are various methods that could be employed to facilitate even greater speed-ups - memory blocking and loop unrolling are two such possibilities. Memory blocking (segmenting the matrices into smaller matrices to further reduce cache misses) was informally experimented with, however, performance differences between it and the current SIMD-infused method were within margin of error for the matrix sizes tested (1,000-5,000), perhaps due to suboptimal blocking technique. It could be worth investigating this further. Regardless of these possible enhancements, however, the speed-ups granted by the integration of intrinsic functions are already substantial in the 80-90% range; in some cases (such as with matrices of sizes near or greater than 10,000x10,000) this makes matrix multiplications feasible which would have been infeasible otherwise.
