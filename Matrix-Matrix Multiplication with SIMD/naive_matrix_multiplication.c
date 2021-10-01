

/*	- THIS IS A TEST FILE	-
	- DO NOT USE - 
	- THE MAIN SIMD_MATRIX.CPP FILE CONTAINS ALL NECESSARY FUNCTIONALITY FOR SIMD AND NORMAL MULTIPLICATION	-	
/*


#include <stdio.h>
#include <time.h>


int main(){
int n = 500;
short int a[n][n];
short int b[n][n];
short int c[n][n];

for (int i = 0; i < n; ++i)
{
	for (int j = 0; j < n; ++j)
	{
		a[i][j] = 7; 
		b[i][j] = 8; 
	}
}
printf("Initialized\n");

// Unoptimized Matrix Multiplication
clock_t Time1 = clock();
for (int i = 0; i < n; ++i)
{
	for (int j = 0; j < n; ++j)
	{
		for (int k = 0; k < n; k++)
		{
      c[i][j] += a[i][k] * b[k][j];
    }
  }
}
clock_t Time2 = clock();


float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
printf("Time taken by loop is %.7f \n", TotalTimeLoop);
return 0;
}

/*	- THIS IS A TEST FILE	-
	- DO NOT USE - 
	- THE MAIN SIMD_MATRIX.CPP FILE CONTAINS ALL NECESSARY FUNCTIONALITY FOR SIMD AND NORMAL MULTIPLICATION	-	
/*
