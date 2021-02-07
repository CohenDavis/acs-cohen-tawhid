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
return;
}

void flo_matrixmult_intrinsic(float** &mat1, float** &mat2, float** &result, int size) { //matrix multiplication using floating-point data and SIMD intrinsics
return;
}


int main(int argc, char** argv){
  int mat_size;
  bool mat_type;
  //User input
  std::cout << "Input maxtrix size (matrix will be square): ";
  std::cin >> mat_size;
  std::cout << "Input data type (0 for fixed-point, 1 for floating point): ";
  std::cin >> mat_type;

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
  //Call corresponding matrix generation functions
  if (mat_type == 0) {
    rand_matrix_fix(mat_1_fix, mat_size);
    rand_matrix_fix(mat_2_fix, mat_size);
    std::cout << "Matrices generated" << std::endl;

    clock_t Time1 = clock();
    fix_matrixmult_normal(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
    clock_t Time2 = clock();
    float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
    std::cout << "Normal multiplication complete" << std::endl;
    printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);

    fix_matrixmult_intrinsic(mat_1_fix, mat_2_fix, mat_result_fix, mat_size);
    std::cout << "SIMD multiplication complete" << std::endl;
    //will need to add timing functionality and/or decide what we want to output
  }
  else {
    rand_matrix_flo(mat_1_flo, mat_size);
    rand_matrix_flo(mat_2_flo, mat_size);
    std::cout << "Matrices generated" << std::endl;

    clock_t Time1 = clock();
    flo_matrixmult_normal(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
    clock_t Time2 = clock();
    float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
    std::cout << "Normal multiplication complete" << std::endl;
    printf("Time taken for normal multiplication is %.7f \n", TotalTimeLoop);

    flo_matrixmult_intrinsic(mat_1_flo, mat_2_flo, mat_result_flo, mat_size);
    std::cout << "SIMD multiplication complete" << std::endl;
    //will need to add timing functionality and/or decide what we want to output
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
