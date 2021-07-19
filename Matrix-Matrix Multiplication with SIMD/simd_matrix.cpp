#include <iostream>
#include <cstdlib>
#include <time.h>
#include <x86intrin.h>


//generate random fixed-point data matrix
void rand_matrix_fix(short int** &input, int size) {
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = rand() % 100;
    }
  }
}

//generate random floating-point data matrix
void rand_matrix_flo(float** &input, int size) {
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = float(rand() % 100);
    }
  }}

//traditional matrix multiplication with fixed-point data
void fix_matrixmult_normal(short int** &mat1, short int** &mat2, short int** &result, int size) {
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

//traditional matrix multiplication with floating-point data
void flo_matrixmult_normal(float** &mat1, float** &mat2, float** &result, int size) {
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

//matrix multiplication using fixed-point data and SIMD intrinsics
void fix_matrixmult_intrinsic(short int** &mat1, short int** &mat2, short int** &result, int size) {
  for (int i=0; i<size; i+=16) {
    for(int j=0; j<size; ++j) {
      //res = result[i][j]
      short int nums[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      __m256i res = _mm256_loadu_si256((__m256i *)nums);
      for(int k=0; k<size; ++k) {
        res = _mm256_add_epi16( //res += mat1[i][k] * mat2[k][j]
          res,
          _mm256_mullo_epi16(
            _mm256_loadu_si256((__m256i *)(&mat1[i][k])),
            _mm256_set1_epi16(mat2[k][j])
            )
        );
      }
      _mm256_storeu_si256((__m256i *)(&mat1[i][j]), res); //result[i,j] = res
    }
  }
}

//matrix multiplication using floating-point data and SIMD intrinsics
void flo_matrixmult_intrinsic(float** &mat1, float** &mat2, float** &result, int size) {

  for (int i=0; i<size; i+=8) {
    for(int j=0; j<size; ++j) {
      //res = result[i][j]
      __m256 res = {0,0,0,0,0,0,0,0};
      for(int k=0; k<size; ++k) {
        res = _mm256_add_ps( //res += mat1[i][k] * mat2[k][j]
          res,
          _mm256_mul_ps(
              _mm256_loadu_ps(&mat1[i][k]),
              _mm256_broadcast_ss(&mat2[k][j])
          )
        );
      }
      _mm256_storeu_ps(&result[i][j], res); //result[i,j] = res
    }
  }
}


int main(int argc, char** argv){
  int mat_size;
  bool mat_type;
  bool mult_type;
  //User input
  std::cout << "Input maxtrix size (matrix will be square): ";
  std::cin >> mat_size;
  std::cout << "Input data type (0 for fixed-point, 1 for floating point): ";
  std::cin >> mat_type;
  std::cout << "Input multiplication type (0 for normal, 1 for with SIMD intrinsics): ";
  std::cin >> mult_type;

  //Initialize possible matrices
  short int** mat_1_fix = new short int*[mat_size];
  short int** mat_2_fix = new short int*[mat_size];
  short int** mat_result_fix = new short int*[mat_size];
  float** mat_1_flo = new float*[mat_size];
  float** mat_2_flo = new float*[mat_size];
  float** mat_result_flo = new float*[mat_size];

  for(int i = 0; i < mat_size; i++){
    mat_1_fix[i] = new short int[mat_size];
    mat_2_fix[i] = new short int[mat_size];
    mat_result_fix[i] = new short int[mat_size];
    mat_1_flo[i] = new float[mat_size];
    mat_2_flo[i] = new float[mat_size];
    mat_result_flo[i] = new float[mat_size];
  }
  std::cout << "Matrices initialized" << std::endl;

  if (mat_type == 0) { // if user wants fixed point mult
    rand_matrix_fix(mat_1_fix, mat_size);
    rand_matrix_fix(mat_2_fix, mat_size);
    std::cout << "Matrices generated" << std::endl;

    if(mult_type == 0){ //if user wants fixed point normal mult
      clock_t Time1 = clock();
      fix_matrixmult_normal(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete multiplication
      std::cout << "Normal multiplication complete" << std::endl;
      printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);

    }
    else { //if user wants fixed point SIMD mult
      clock_t Time1 = clock();
      fix_matrixmult_intrinsic(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "SIMD multiplication complete" << std::endl;
      printf("Time taken for SIMD multiplication is %.7f \n", TotalTimeLoop);
    }

  }
  else { //if user wants floating point mult
    rand_matrix_flo(mat_1_flo, mat_size);
    rand_matrix_flo(mat_2_flo, mat_size);
    std::cout << "Matrices generated" << std::endl;

    if(mult_type == 0){ //if user wants floating point normal mult
      clock_t Time1 = clock();
      flo_matrixmult_normal(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "Normal multiplication complete" << std::endl;
      printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);
    }
    else { //if user wants floating point SIMD mult
      clock_t Time1 = clock();
      flo_matrixmult_intrinsic(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "SIMD multiplication complete" << std::endl;
      printf("Time taken for SIMD multiplication is %.7f \n", TotalTimeLoop);
    }

  }

  //indicates multiplication/process finished
  std::cout << "Process Complete" << std::endl;

  return 0;
}
