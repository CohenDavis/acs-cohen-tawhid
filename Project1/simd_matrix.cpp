#include <iostream>
#include <cstdlib>
#include <x86intrin.h>



void rand_matrix_fix(short int &input, int size) { //generate random fixed-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = rand() % 100;
    }
  }
}

void rand_matrix_flo(float &input, int size) { //generate random floating-point data matrix
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++) {
      input[i][j] = float(rand() % 100);
    }
  }}

void fix_matrixmult_normal(short int &mat1, short int &mat2, short int &result) { //traditional matrix mutliplication with fixed-point data

}

void flo_matrixmult_normal(short int &mat1, short int &mat2, short int &result) { //traditional matrix multiplication with floating-point data

}

void fix_matrixmult_intrinsic(float &mat1, float &mat2, float &mat3) { //matrix multiplication using fixed-point data and SIMD intrinsics

}

void flo_matrixmult_intrinsic(float &mat1, float &mat2, float &mat3) { //matrix multiplication using floating-point data and SIMD intrinsics

}


int main(int argc, char** argv){
  int mat_size
  bool mat_type
  //User input
  std::cout << "Input maxtrix size (matrix will be square): ";
  std::cin >> mat_size;
  std::cout << "Input data type (0 for fixed-point, 1 for floating point): ";
  std::cin >> mat_type;

  //Initialize possible matrices
  short int mat_1_fix[mat_size][mat_size];
  short int mat_2_fix[mat_size][mat_size];
  short int mat_result_fix[mat_size][mat_size];
  float mat_1_flo[mat_size][mat_size];
  float mat_2_flo[mat_size][mat_size];
  float mat_result_flo[mat_size][mat_size];

  //Call corresponding matrix generation functions
  if (mat_type == 0) {
    rand_matrix_fix(mat_1_fix, mat_size);
    rand_matrix_fix(mat_2_fix, mat_size);
    fix_matrixmult_normal(mat_1_fix, mat_2_fix, mat_result_fix);
    fix_matrixmult_intrinsic(mat_1_fix, mat_2_fix, mat_result_fix);
    //will need to add timing functionality and/or decide what we want to output
  }
  else {
    rand_matrix_flo(mat_1_flo, mat_size);
    rand_matrix_flo(mat_2_flo, mat_size);
    flo_matrixmult_normal(mat_1_flo, mat_2_flo, mat_result_flo);
    flo_matrixmult_intrinsic(mat_1_flo, mat_2_flo, mat_result_flo);
    //will need to add timing functionality and/or decide what we want to output
  }




  return 0;
}
