#include <iostream>
#include <cstdlib>
#include <time.h>
#include <x86intrin.h>



void rand_matrix_fix(short int** &input, int size) { //generate random fixed-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = rand() % 100;
    }
  }
}

void rand_matrix_flo(float** &input, int size) { //generate random floating-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = float(rand() % 100);
    }
  }}

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

void flo_matrixmult_normal(float** &mat1, float** &mat2, float** &result, int size) { //traditional matrix multiplication with floating-point data
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

void fix_matrixmult_intrinsic(short int** &mat1, short int** &mat2, short int** &result, int size) { //matrix multiplication using fixed-point data and SIMD intrinsics
  for (int i=0; i<size; i+=8) {
    for(int j=0; j<size; ++j) {
      //res = result[i][j]
      __m256 res = {0,0,0,0,0,0,0,0};
      for(int k=0; k<size; ++k) {
        //__m256i mat1_val = _mm_load_si128(&mat1[i][k]);
        float mat1_val = (float)(mat1[i][k]);
        float mat2_val = (float)(mat2[k][j]);
        res = _mm256_add_ps(
          res,
          _mm256_mul_ps(
            _mm256_load_ps(&mat1_val),
            _mm256_broadcast_ss(&mat2_val)
            )
        );
      }
      for (int ii=i-8; ii<i; ii++){
        result[i][j] = res[ii];
      }
    }
  }
}

void flo_matrixmult_intrinsic(float** &mat1, float** &mat2, float** &result, int size) { //matrix multiplication using floating-point data and SIMD intrinsics

  for (int i=0; i<size; i+=8) {
    for(int j=0; j<size; ++j) {
      //res = result[i][j]
      __m256 res = {0,0,0,0,0,0,0,0};
      for(int k=0; k<size; ++k) {
        res = _mm256_add_ps(
          res,
          _mm256_mul_ps(
            _mm256_loadu_ps(&mat1[i][k]),
            _mm256_broadcast_ss(&mat2[k][j])
            )
        );
      }
      for (int ii=i-8; ii<i; ii++){
        result[i][j] = res[ii];
      }
        
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

  if (mat_type == 0) { //fixed point mult
    rand_matrix_fix(mat_1_fix, mat_size);
    rand_matrix_fix(mat_2_fix, mat_size);
    std::cout << "Matrices generated" << std::endl;

    if(mult_type == 0){ //normal mult
      clock_t Time1 = clock();
      fix_matrixmult_normal(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "Normal multiplication complete" << std::endl;
      printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);

    }
    else { //SIMD mult
      clock_t Time1 = clock();
      fix_matrixmult_intrinsic(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "SIMD multiplication complete" << std::endl;
      printf("Time taken for SIMD multiplication is %.7f \n", TotalTimeLoop);
    }

  }
  else { //floating point mult
    rand_matrix_flo(mat_1_flo, mat_size);
    rand_matrix_flo(mat_2_flo, mat_size);
    std::cout << "Matrices generated" << std::endl;

    if(mult_type == 0){ //normal mult
      clock_t Time1 = clock();
      flo_matrixmult_normal(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "Normal multiplication complete" << std::endl;
      printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);
    }
    else { //SIMD mult
      clock_t Time1 = clock();
      flo_matrixmult_intrinsic(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
      clock_t Time2 = clock();
      float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
      std::cout << "SIMD multiplication complete" << std::endl;
      printf("Time taken for SIMD multiplication is %.7f \n", TotalTimeLoop);
    }

  }

  //Heap Cleanup
  for(int i = 0; i < mat_size; i++){
    delete [] mat_1_fix[i];
    delete [] mat_2_fix[i];
    delete [] mat_result_fix[i];
    delete [] mat_1_flo[i];
    delete [] mat_2_flo[i];
    delete [] mat_result_flo[i];
  }
  delete [] mat_1_fix;
  delete [] mat_2_fix;
  delete [] mat_result_fix;
  delete [] mat_1_flo;
  delete [] mat_2_flo;
  delete [] mat_result_flo;

  std::cout << "Process Complete" << std::endl;

  return 0;
}
